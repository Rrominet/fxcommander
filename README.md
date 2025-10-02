# FxCommander

It's a really lightway and fast application launcher for linux.
The Bonus is that you can your custom commands really simply.

[](https://motion-live.com/fxos/data/fxcommander-demo.mp4)

## Dependency Graph

[cpp-utils](https://github.com/Rrominet/cpp-utils) <- [fxos_gui-lib](https://github.com/Rrominet/fxos_gui-lib) <- [fxcommander](https://github.com/Rrominet/fxcommander)  
[cpp-utils](https://github.com/Rrominet/cpp-utils) <- [fxlauncher-backend](https://github.com/Rrominet/fxlauncher-backend) <- [fxcommander](https://github.com/Rrominet/fxcommander)

*But don't worry about it, the installation scripts below manage all for you :)*

## Installation

Just run the script `build-n-install` as `sudo`.  
For you convenience, just copy the code below and all should be done automatically (cloning repos, installing dependencies, compiling, installing...).
```bash 
git clone https://github.com/Rrominet/fxcommander.git
cd ./fxcommander
sudo chmod +x ./build-n-install
sudo ./build-n-install
```

Annnnd you're done !
> [!WARNING]
> For now the `build-n-install` script is bind to debian based distros (it uses `apt`) but I will port it soon on any others.
> So Stay tuned for that :)

## Usage

Once installed you can launch FxLauncher by the command `fxcommander` (pointing to `/usr/lib/local/fxcommander`)

Of course launching like this is a bit dumb.  
What you want to do is to linked this command `fxcommander` to a global shortcut on your desktop environment.

If you want a portable way of doing it that will work anywhere (on X11 **not Wayland**), you can use [sxhkd](https://github.com/baskerville/sxhkd).  
Or just use any other system, that's not what missing : 
 - Ubuntu Desktop : `Settings` -> `Keyboard` -> `View and Customize Shortcuts` -> `Custom Shortcuts` -> `Add Shortcut` 
 - KDE Plasma : `System Settings` -> `Shortcuts` -> `Custom Shortcuts`
 - Xfce : `System Settings` -> `Keyboard` -> `Application Shortcuts` -> `Add`
 ...

First time you open the app, you'll have no applciation to run. It's normal you need to Reload the Desktop application simply by Pressing `Ctrl` + `R`.  
And you're good to go you can tyoe anything you want and press `Enter` to open your app.

[](https://motion-live.com/fxos/data/fxcommander-reload.mp4)

## Custom commands

You can add custom commands by searching gor `Create Command` or by pressing `Ctrl` + `N`.  
From there put : 
- a Name, 
- any alliases (word you can type in the search bar to find it)
- an ID (not-mendatory)
- A description
- The *Process Command*, this what actually get executed.
- A Keybind if you want
- And if you want to exectute this command in a terminal or not.

Press `Valid` and you good to go.

> [!NOTE]
> If you install new apps, just press `Ctrl` + `R` to reload them and get them to show.
