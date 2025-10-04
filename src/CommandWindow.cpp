#include "./CommandWindow.h"
#include "mlgui.2/src/App.h"

#include "mlgui.2/src/Box.hpp"
#include "mlgui.2/src/Scrollable.hpp"

#include "./FxCommanderApp.h"

#include "./PrefsWindow.h"
#include "./MainWindow.h"

#include "storage.h"

CommandWindow::CommandWindow(ml::App* app,Type type) : ml::Window(app), _type(type), _name("Name"), _aliases("Aliases"), _cmdid("Id"), _process("Process Command"), _description("Description"), _keybind("Keybind"), _useTerminal("Use terminal"), _removeMainWindow("Close Commander after", true)
{
    
}

CommandWindow::CommandWindow(ml::App* app,ml::Window* parent,Type type) : ml::Window(app, parent), _type(type), _name("Name"), _aliases("Aliases"), _cmdid("Id"), _process("Process Command"), _description("Description"), _keybind("Keybind"), _useTerminal("Use terminal"), _removeMainWindow("Close Commander after", true)
{
    
}

void CommandWindow::init()
{
    ml::Window::init();
    if (_type == ADD)
        this->setTitle("Add Command");
    else if (_type == MODIFY)
        this->setTitle("Modify Command");

    _aliases.setDescription("Set of different names that you can use to call this command.\nSeparate them by a comma, spaces will not be taken into account.");
    _process.setDescription("The process command that you would write in a sh shell.");

    _main->addCssClass("command-window");
    _main->appendProp<ml::StringProperty>(&_name);
    _main->appendProp<ml::StringProperty>(&_aliases);
    _main->appendProp<ml::StringProperty>(&_cmdid);
    _main->appendProp<ml::StringProperty>(&_process, ml::MULTILINE_ENTRY);
    _main->appendProp<ml::StringProperty>(&_description, ml::MULTILINE_ENTRY);
    _main->appendProp<ml::StringProperty>(&_keybind);

    auto _checks = _main->createBox();
    _checks->setOrient(ml::HORIZONTAL);
    _checks->addCssClass("commander-checks");
    _checks->appendProp<ml::BoolProperty>(&_useTerminal, ml::TOGGLEBUTTON);
    _checks->appendProp<ml::BoolProperty>(&_removeMainWindow, ml::TOGGLEBUTTON);

    _valid = _main->createButton("Valid");
    _valid->addCssClass("commander-valid");
    _valid->setHAlign(ml::FILL);
    _valid->setHExpand();
    _valid->setVExpand();

    _setEvents();

    this->setSize(601, 590);
    this->hideOnClose(false);
}

void CommandWindow::createCommand()
{
    _cmd = ml::app()->cmds().createCommand<ml::ProcessCommand>(_name.value()).get();
    this->updateCommandFromUI();
    commander::get()->commanderW()->commander()->addCommand(_cmd);

    this->destroy();
}

void CommandWindow::updateCommandFromUI()
{
    if (!_cmd)
    {
        ml::app()->error("Can't update the command from the GUI, no command setted.");
        return;
    }
    if (!_aliases.value().empty())
    {
        std::string _tmp = str::replace(_aliases.value(), " ", "");
        _cmd->aliases().clear();
        _cmd->aliases().concat(str::split(_tmp, ","));
    }

    if (!_cmdid.value().empty())
        _cmd->setId(_cmdid.value());

    std::string process = _process.value();
    if (_useTerminal.value())
        process = commander::get()->prefsW()->terminalCmd().value() + " " + process;

    _cmd->setProcessPath(process);
    _cmd->setHelp(_description.value());
    _cmd->setKeybind(_keybind.value());

    _cmd->userData()["removeMainWindow"] = _removeMainWindow.value();

    storage::set("commands", ml::app()->cmds().serialize());
}

void CommandWindow::modifyCommand()
{
    if (!_cmd)
    {
        ml::app()->error("No active command to modify.");
        return;
    }
    this->updateCommandFromUI();
    this->destroy();
}

void CommandWindow::valid()
{
    if (_type == ADD)
        this->createCommand();
    else if (_type == MODIFY)
        this->modifyCommand();
}

void CommandWindow::_setEvents()
{
    _valid->addEventListener(ml::LEFT_UP, [this](ml::EventInfos&){this->valid();});
}

void CommandWindow::setType(Type type)
{
    _type = type;	
    if (_type == ADD)
        this->setTitle("Add Command");
    else if (_type == MODIFY)
        this->setTitle("Modify Command");
}

void CommandWindow::setCommand(ml::ProcessCommand* cmd)
{
    _cmd = cmd;
    _name.set(cmd->name());

    std::string aliases = "";
    for (auto& a : cmd->aliases())
        aliases += a + ", ";

    aliases = aliases.substr(0, aliases.size() - 2);
    _aliases.set(aliases);
    _cmdid.set(cmd->id());
    _description.set(cmd->help());
    _keybind.set(cmd->keybind());

    if (str::contains(cmd->processPath(), commander::get()->prefsW()->terminalCmd().value()))
    {
        _useTerminal.set(true);
        std::string process = str::replace(cmd->processPath(), commander::get()->prefsW()->terminalCmd().value() + " ", "");
        _process.set(process);
    }
    else 
    {
        _useTerminal.set(false);
        _process.set(cmd->processPath());
    }

    if (cmd->userData().contains("removeMainWindow"))
        _removeMainWindow.set(cmd->userData()["removeMainWindow"].get<bool>());	
}
