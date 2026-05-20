#pragma once
#include "mlgui.2/src/App.h"
#include "mlprocess.h"

class MainWindow;
class PrefsWindow;
class FxCommanderApp : public ml::App
{
    public:
        FxCommanderApp(int argc, char *argv[]);
        virtual ~FxCommanderApp();
        void createCommands();
        void createWindows();
        void setEvents();

        void onAbout();
        void onHelp();
        void onReloadDesktops();
        bool commandExists(const std::string& id);

        void increaseCommandScore(ml::Command* command, float toAdd);
        void setProgramExt(ml::Command* command, const std::string& ext);

        //return true if there is a ext mapped.
        bool openFileIfExtMapped();
        void exec(const std::string& program, const std::string& file);

    private : 
        MainWindow* _commanderW=nullptr; //bp cg
        PrefsWindow* _prefsW = nullptr; //bp cg

        Process* _fxlauncher = nullptr; //bp cg
        bool _do_nothing = false;

        json _commandsScores; //bp cg

        // setted via argv (first one).
        // if empty, default behavior
        std::string _fileToOpen; //bp cg
        json _programExtMap;

        json _commandsData;
        bool _changeSoftware = false; //bp cg

    public : 
#include "./FxCommanderApp_gen.h"
};

namespace commander
{
    FxCommanderApp* get();
}
