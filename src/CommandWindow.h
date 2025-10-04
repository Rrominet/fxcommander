#pragma once

#include "mlgui.2/src/Window.h"
#include "mlgui.2/src/Property.h"

#include "commands/ProcessCommand.h"

namespace ml
{
    class Button;
}

class CommandWindow : public ml::Window
{
    public : 
        enum Type
        {
            ADD, 
            MODIFY
        };

        CommandWindow(ml::App* app, Type type=ADD);
        CommandWindow(ml::App* app, ml::Window* parent, Type type=ADD);

        virtual ~CommandWindow() = default;

        virtual void init() override;

        void createCommand();
        void modifyCommand();

        // this is what is executed when _valid is clicked
        void valid();

        void setType(Type type);
        void setCommand(ml::ProcessCommand* cmd);

        void updateCommandFromUI();

    protected : 

        ml::StringProperty _name;
        ml::StringProperty _aliases;
        ml::StringProperty _cmdid;
        ml::StringProperty _process; //the path and the args written like in a terminal sh
        ml::StringProperty _description;
        ml::StringProperty _keybind;
        ml::BoolProperty _useTerminal;

        ml::BoolProperty _removeMainWindow;

        Type _type = ADD; //bp cg

        std::shared_ptr<ml::Button> _valid = nullptr;

        void _setEvents();
        ml::ProcessCommand* _cmd = nullptr;

#include "./CommandWindow_gen.h"

};
