# directZob
A full software 3D engine

## Requirements

On windows, vs2017 or the "Build Tools for Visual Studio 2017 (version 15.0)", and the windows SDK are required to compile the project. Downloads are available here :
https://my.visualstudio.com/Downloads?q=Visual%20Studio%202017


To build the engine solution, you will need to have gyp installed on your machine.
gyp can be found as packages for linux distributions, otherwise it can be installed with pip.

Make sure that you installed python v3, and pip, maybe through the official install :
https://pip.pypa.io/en/stable/installing/

For some reason, the gyp package in pip repository may not want to install. Installing the latest version can be done manually with the command :
pip install git+https://chromium.googlesource.com/external/gyp

On linux, compilation is supported with gcc 7.5. The following additional packages are needed : libx11-dev, libxml2-dev

in order to compile properly you must have Autodesk FBX Software Developer Kit installed and copy fbxsdk.dll into the application directory of the engine or editor executable 

## Gyp documentation

VC++ compiler flags:
https://github.com/chromium/gyp/blob/master/pylib/gyp/MSVSSettings_test.py


