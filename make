#!/usr/bin/env python3

from ml import build
from ml.boilerplate import cpp
from ml import fileTools as ft
import os
import sys
sys.path.append("../")

fm = "/media/romain/Donnees/Programmation/cpp/frameworks"
libs = "/media/romain/Donnees/Programmation/cpp/libs"
prjs = "/media/romain/Donnees/Programmation/cpp/cmake"

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

if not cpp.release : 
    cpp.addToLibs([
        "stdc++fs",
        fm + "/build/libmlapi.so",
        prjs + "/mlgui.2/build/libmlgui.so",
        ])

elif cpp.release : 
    cpp.addToLibs([
        "stdc++fs",
        "mlapi",
        "/opt/mlgui/lib/libmlgui.so",
        ])

if("clean" in sys.argv or "clear" in sys.argv):
    cpp.clean()
    exit()
else:
    cpp.build()
