#!/usr/bin/env python3

from ml import build
from ml.boilerplate import cpp
from ml import fileTools as ft
import os
import sys
sys.path.append("../")

fm = "../../../frameworks"
libs = "../../../libs"
prjs = "../.."

for arg in sys.argv:
    if "libs=" in arg:
        libs = arg.split("=")[1]
    elif "fm=" in arg or "cpp-utils=" in arg:
        fm = arg.split("=")[1]
    elif "prjs=" in arg:
        prjs = arg.split("=")[1]

cpp.generate("../src")

includes = [
    "../src",
    libs + "/json",
    libs + "/eigen",
    fm,
   prjs,
        ]

srcs = [
        "../src",
        ]

libs = [
        ]

defs = ["NO_LOG"]

cpp = build.create("fxcommander", sys.argv)
cpp.static = False
cpp.includes = includes
cpp.addToSrcs(srcs)
cpp.addToLibs(libs)
cpp.definitions += defs
cpp.addInstalledLibrary("gtkmm-4.0")
# sudo apt install libwebkitgtk-6.0-dev
cpp.addInstalledLibrary("webkitgtk-6.0")

cpp.addToLibs("stdc++fs")
cpp.addToLibs("boost_filesystem")

if "release" in sys.argv:
    cpp.addToLibs(fm + "/build/libmlapi.a")
    cpp.addToLibs(prjs + "/mlgui.2/build/libmlgui.a")
else : 
    cpp.addToLibs(fm + "/build/libmlapi_debug.a")
    cpp.addToLibs(prjs + "/mlgui.2/build/libmlgui_debug.a")

if("clean" in sys.argv or "clear" in sys.argv):
    cpp.clean()
    exit()
else:
    cpp.build()
