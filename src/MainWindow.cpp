#include "./MainWindow.h"
#include "./FxCommanderApp.h"

#include "./PrefsWindow.h"

#include "commands/ProcessCommand.h"
#include "mlgui.2/src/Scrollable.hpp"

MainWindow::MainWindow(ml::App* app) : ml::Window(app){}
MainWindow::MainWindow(ml::App* app, ml::Window* parent) : ml::Window(app, parent){}
MainWindow::~MainWindow(){}

void MainWindow::init()
{
    ml::Window::init();	
    this->setTitle("FxCommander");
    this->createMenus();
    this->setSize(729, 275);

    _main->createLabel("Execute any command you want.");
    _commander = _main->createComposedWidget<ml::Commander>(&_main->content());
    _commander->addAllCommands(ml::app());

    _setEvents();

    this->hideOnClose(false);

    this->addOnClose([this]{
            commander::get()->prefsW()->destroy();
            });
}

void MainWindow::_setEvents()
{
    _commander->events().add("context", [this]
    {
        auto cmd = std::any_cast<ml::Command*>(_commander->events().data()); 
        _activeCmd = cmd;
        this->showMenu("command-context");
    });

    _commander->events().add("valid", [this]
            {
                auto cmd = std::any_cast<ml::Command*>(_commander->events().data()); 
                auto pcmd = static_cast<ml::ProcessCommand*>(cmd);
                if (cmd->userData().contains("removeMainWindow") && cmd->userData()["removeMainWindow"].get<bool>())
                {
                    ml::app()->quit();
                }
            });
}

void MainWindow::createMenus()
{
    auto ctx = _app->menusFactory().create("command-context", "Command Context");
    ctx->addCommand("modify-cmd");
    ctx->addCommand("remove-cmd");
}

