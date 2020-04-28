# directZob
A full software 3D engine

To build the engine solution, you will need to have gyp installed on your machine.
gyp can be found as packages for linux distributions, otherwise it can be installed with pip.
For one reason, the package in pip repository does not want to install. Installing the latest version can be done manually with the command :
pip install git+https://chromium.googlesource.com/external/gyp

On linux, compilation is supported with gcc 7.5. The following additional packages are needed : libx11-dev, libxml2-dev

in order to compile properly you must have Autodesk FBX Software Developer Kit installed and copy fbxsdk.dll into the application directory of the engine or editor executable 
