pip3 install --upgrade six>=1.13.0
pip3 install gyp-next
gyp .\DirectZob.gyp --depth=. --generator-output=build/DirectZob/ -f msvs -G msvs_version=2022


#(Get-Content .\build\DirectZob\DirectZobDInterop.vcxproj).Replace('<RootNamespace>DirectZobDInterop</RootNamespace>', '<RootNamespace>DirectZobDInterop</RootNamespace><ManagedAssembly>true</ManagedAssembly>') | Set-Content .\build\DirectZob\DirectZobDInterop.vcxproj
