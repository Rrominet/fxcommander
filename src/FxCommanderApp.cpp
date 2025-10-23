#include "./FxCommanderApp.h"
#include "./MainWindow.h"
#include "./PrefsWindow.h"
#include "mlgui.2/src/GuiCommand.h"
#include "mlgui.2/src/GuiBackendCommand.h"

#include "debug.h"
#include "mlgui.2/src/App.hpp"
#include "mlgui.2/src/IconMessageDialog.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "./CommandWindow.h"

#include "storage.h"
#include "ipc.h"
#include "freedesktop.h"

namespace commander
{
    FxCommanderApp* _commander = nullptr;
}

FxCommanderApp::FxCommanderApp(int argc,char *argv[]) : ml::App(argc,argv)
{
    commander::_commander = this;	
    storage::init();
    
    _do_nothing = this->argv().has("do-nothing");

    this->createCommands();
    this->createWindows();
    this->setEvents();
    db::setLogFile(files::execDir() + files::sep() + "fxcommander.log");

    auto later = [this]
    {
        this->addCss("fxcommander.css");
    };

    this->setTimeout(later, 16);
}

void FxCommanderApp::setEvents()
{
}

void FxCommanderApp::createCommands()
{
    auto cmd = _cmds.createCommand<ml::GuiCommand>("Create Command", "create-cmd");
    cmd->aliases().concat({"ccmd", "ncmd"});
    cmd->setHelp("Create a new command to execute...");
    cmd->setKeybind("ctrl n");
    cmd->setExec([this](const std::any&){auto w = this->createWindow<CommandWindow>(_main); w->show();});

    cmd = _cmds.createCommand<ml::GuiCommand>("Modify Command", "modify-cmd");
    cmd->setHelp("Modify the active command.");
    cmd->setExec([this](const std::any&){
                if (!_commanderW->activeCmd())
                {
                    this->error("No active command to modify.");
                    return;
                }
                auto w = this->createWindow<CommandWindow>(_commanderW);
                w->setType(CommandWindow::MODIFY);
                w->setCommand(static_cast<ml::ProcessCommand*>(_commanderW->activeCmd()));
                w->show();
            });

    cmd = _cmds.createCommand<ml::GuiCommand>("Remove Command", "remove-cmd");
    cmd->setHelp("Remove the active command.");
    cmd->setExec([this](const std::any&){
                if (!_commanderW->activeCmd())
                {
                    this->error("No active command to remove.");
                    return;
                }
                _commanderW->commander()->removeCommand(_commanderW->activeCmd());
                _cmds.removeCommand(_commanderW->activeCmd()->id());
                storage::set("commands", this->cmds().serialize());
            });

    cmd = _cmds.createCommand<ml::GuiCommand>("About", "about");
    cmd->setHelp("Show the about dialog...");
    cmd->setExec([this](const std::any&){this->onAbout();});

    cmd = _cmds.createCommand<ml::GuiCommand>("Quit", "quit");
    cmd->aliases().concat({"exit"});
    cmd->setHelp("Quit the application...");
    cmd->setKeybind("ctrl q");
    cmd->setExec([this](const std::any&){this->quit();});

    cmd = _cmds.createCommand<ml::GuiCommand>("Help", "help");
    cmd->setHelp("Show the help dialog...");
    cmd->setKeybind("ctrl h");
    cmd->setExec([this](const std::any&){this->onHelp();});

    cmd = _cmds.createCommand<ml::GuiCommand>("Preferences", "prefs");
    cmd->setHelp("Show the preferences dialog...");
    cmd->setKeybind("ctrl comma");
    cmd->setExec([this](const std::any&){_prefsW->show(); _prefsW->focus();});

    cmd = _cmds.createCommand<ml::GuiCommand>("Reload Desktops files", "reload-desktops");
    cmd->aliases().concat({"rldesktops", "rdktps", "rapps", "reloadapps"});
    cmd->setHelp("Reload desktops files installed on the local machine and transform them into command that you can call.");
    cmd->setKeybind("ctrl r");
    cmd->setExec([this](const std::any&){this->onReloadDesktops();});

    json data = storage::get<json>("commands");
    _cmds.deserialize<ml::GuiCommand, ml::GuiBackendCommand>(data);
}

void FxCommanderApp::createWindows()
{
    this->createMainWindow<MainWindow>();
    _commanderW = (MainWindow*)_main;

    _commanderW->commander()->setDoNothing(_do_nothing);

    _prefsW = this->createWindow<PrefsWindow>(_main).get();
    _prefsW->setStayHiddenFirstRun(true);
}

FxCommanderApp::~FxCommanderApp()
{
    commander::_commander = nullptr;	
}

void FxCommanderApp::onAbout()
{
    auto dlg = this->info("You can add and execute any command you want as if you were in a shell.\nHere you have a better search system and more control on how the command are executed.\n\nCredits : Motion Live", _commanderW);
    dlg->setTitle("About");
}

void FxCommanderApp::onHelp()
{
    auto dlg = this->info("Add any command/executale on your system with the command 'Create Command'.\nYou can modify or remove a command by right click on it.\n\nThe rest is pretty straight forward.", _commanderW);	
    dlg->setTitle("Help");
}

bool FxCommanderApp::commandExists(const std::string& id)
{
    try
    {
        _cmds.command(id);
        return true;
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

void FxCommanderApp::onReloadDesktops()
{
    if (!_fxlauncher)
        _fxlauncher = process::start("fxlauncher");

    json args;
    args["length"] = 9999;
    _main->setWorking(true);

    auto cb = [this](const json& r)
    {
        if (!r.contains("data"))
        {
            lg("No data");
            return;
        }
        if (!r["success"])
        {
            lg("No success");
            return;
        }

        for (auto& d : r["data"])
        {
            if (!d.contains("entries"))
            {
                lg("No entries in command so we're skipping it.");
                continue;
            }

            auto props = d["entries"];
            if (!props.contains("Name"))
            {
                lg("No Name in command so we're skipping it.");
                continue;
            }

            ml::Command* cmd = nullptr;
            ml::ProcessCommand* pcmd = nullptr;
            if (this->commandExists(props["Name"]))
            {
                lg("Command " << props["Name"] << " already exists so we're getting it.");
                cmd = this->cmds().command(props["Name"]).get();
                pcmd = dynamic_cast<ml::ProcessCommand*>(cmd);
                if (!pcmd)
                {
                    lg("Command " << props["Name"] << " is not a process command so we're skipping it.");
                    continue;
                }
            }
            else 
            {
                lg("Command " << props["Name"] << " doesn't exist so we're creating it.");
                pcmd = this->cmds().createCommand<ml::ProcessCommand>(props["Name"]).get();
            }

            try
            {
                std::string pc = freedesktop::execNoCodes(props["Exec"]);
                if (props.contains("Terminal") && props["Terminal"] == "true") 
                    pc = this->prefsW()->terminalCmd().value() + " " + pc;
                pcmd->setProcessPath(pc);

                if (props.contains("Comment"))
                    pcmd->setHelp(props["Comment"]);
                pcmd->userData()["removeMainWindow"] = true;

            }
            catch(const std::exception& e)
            {
                lg("Failed to complete command " << props["Name"] << " : " << e.what());
                continue;
            }
        }

        storage::set("commands", this->cmds().serialize());
        this->commanderW()->commander()->clear();
        this->commanderW()->commander()->addAllCommands(this);
        _main->setWorking(false);
    };

    ipc::call(_fxlauncher, "getFromFilesystem", args, [this, cb](const json& r)
            {
                lg(r.dump(4));
                this->queue([cb, r]{cb(r);});
            });
}

namespace commander
{
    FxCommanderApp* get(){return _commander;}
}

