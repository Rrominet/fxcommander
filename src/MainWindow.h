#pragma once
#include "mlgui.2/src/Window.h"
#include "mlgui.2/src/Commander.h"
#include "commands/Command.h"

namespace ml
{
    class Label;
    class Box;
    class Subrange;
}

class MainWindow : public ml::Window
{
    public:
        MainWindow(ml::App* app);
        MainWindow(ml::App* app, ml::Window* parent);
        virtual ~MainWindow();

        virtual void init() override;
        void createMenus();

    protected : 
        void _setEvents();

        std::shared_ptr<ml::Commander> _commander = nullptr; //bp cg
                                                             //
        ml::Command* _activeCmd = nullptr; //bp cg

    public : 
#include "./MainWindow_gen.h"
};
