# directZob
A full software 3D engine

Windows : [![Windows](https://dev.azure.com/zeralph/DirectZob/_apis/build/status/azure_windows_build?branchName=master)](https://dev.azure.com/zeralph/DirectZob/_build/latest?definitionId=6&branchName=master) Linux : [![Build Status](https://dev.azure.com/zeralph/DirectZob/_apis/build/status/azure_linux_build?branchName=master)](https://dev.azure.com/zeralph/DirectZob/_build/latest?definitionId=5&branchName=master) MacOs: [![Build Status](https://dev.azure.com/zeralph/DirectZob/_apis/build/status/azure_macos_build?branchName=master)](https://dev.azure.com/zeralph/DirectZob/_build/latest?definitionId=3&branchName=master)

## Requirements

### gyp build system

To build the engine solution, you will need to have gyp installed on your machine.
gyp can be found as packages for linux distributions, otherwise it can be installed with pip.

Make sure that you installed python v3, and pip, maybe through the official install :
https://pip.pypa.io/en/stable/installing/

For some reason, the gyp package in pip repository may not want to install. Installing the latest version can be done manually with the command :
pip install git+https://chromium.googlesource.com/external/gyp


### Windows

On windows, vs2017 or the "Build Tools for Visual Studio 2017 (version 15.0)" aka MsBuild, and the windows SDK are required to compile the project. Downloads are available here :
https://my.visualstudio.com/Downloads?q=Visual%20Studio%202017

When using MsBuild, make sure C++ /CLI support is installed and that msbuild is part of the PATH, then run buildMsBuild.bat; if any errors regarding .NET framework versions happens, tweak the framework version passed as argument in the bat file to the installed version

### Linux

On linux, compilation is supported with gcc 7.5. The following additional packages are needed : libx11-dev, libxml2-dev


## Gyp documentation

VC++ compiler flags:
https://github.com/chromium/gyp/blob/master/pylib/gyp/MSVSSettings_test.py


