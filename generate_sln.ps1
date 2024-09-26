
pip3 install gyp-next
gyp .\DirectZob.gyp --depth=. --generator-output=build/ -f msvs -G msvs_version=2022


#(Get-Content .\build\DirectZob\DirectZobDInterop.vcxproj).Replace('<RootNamespace>DirectZobDInterop</RootNamespace>', '<RootNamespace>DirectZobDInterop</RootNamespace><ManagedAssembly>true</ManagedAssembly>') | Set-Content .\build\DirectZob\DirectZobDInterop.vcxproj
