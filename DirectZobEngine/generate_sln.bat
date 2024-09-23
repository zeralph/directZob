rem pip3 install --upgrade six>=1.13.0

rem echo "Do not use me, use generate_sln.ps1 instead"
powershell ./generate_sln.ps1
rem pip3 install gyp-next
rem  gyp .\DirectZob.gyp --depth=. --generator-output=build/DirectZob/ -f msvs -G msvs_version=2022

rem  <ManagedAssembly>true</ManagedAssembly>
