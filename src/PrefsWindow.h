#pragma once

#include "mlgui.2/src/Window.h"
#include "mlgui.2/src/Property.h"

namespace ml
{
    class Button;
}

class PrefsWindow : public ml::Window
{
    public : 
        PrefsWindow(ml::App* app);
        PrefsWindow(ml::App* app, ml::Window* parent);

        virtual void init() override;

        virtual ~PrefsWindow() = default;

        json serialize();
        void deserialize(const json& j) override;

        void read();

    protected : 
        ml::StringProperty _terminalCmd; //bp cg

        void _setEvents();

        // save all the prefs in storage
        void _onPropChange();

    public : 
        #include "./PrefsWindow_gen.h"

};
