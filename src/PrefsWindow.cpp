#include "./PrefsWindow.h"

#include "mlgui.2/src/App.h"

#include "mlgui.2/src/Box.hpp"
#include "mlgui.2/src/Scrollable.hpp"

#include "storage.h"

PrefsWindow::PrefsWindow(ml::App* app) : ml::Window(app), _terminalCmd("Terminal Command")
{
	
}

PrefsWindow::PrefsWindow(ml::App* app,ml::Window* parent) : ml::Window(app, parent), _terminalCmd("Terminal Command")
{
	
}

void PrefsWindow::init()
{
    ml::Window::init();	
    this->setTitle("Preferences");

    _terminalCmd.setDescription("If a command should be executed in a terminal, this the command that should open the terminal and exec your command\nFor example : gonme-terminal --");

    _main->appendProp<ml::StringProperty>(&_terminalCmd, ml::MULTILINE_ENTRY);

    this->read();
    _setEvents();

    this->setSize(600, 400);
}

void PrefsWindow::_setEvents()
{
    _terminalCmd.addOnUpdate(std::bind(&PrefsWindow::_onPropChange, this));	
    _terminalCmd.addOnValid(std::bind(&PrefsWindow::_onPropChange, this));	
}

void PrefsWindow::_onPropChange()
{
    storage::set("prefs", this->serialize());
}

json PrefsWindow::serialize()
{
    json _r = json::object();	
    _r["terminalCmd"] = _terminalCmd.value();
    return _r;
}

void PrefsWindow::deserialize(const json& j)
{
    if (j.contains("terminalCmd"))
        _terminalCmd.set(j["terminalCmd"].get<std::string>());	
}

void PrefsWindow::read()
{
    json data = storage::get<json>("prefs");	
    this->deserialize(data);

}
