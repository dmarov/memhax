call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cmake -G "Ninja" ..  "-DVCPKG_TARGET_TRIPLET=x64-windows-static" "-DCMAKE_EXPORT_COMPILE_COMMANDS=1" "-DCMAKE_BUILD_TYPE=Release"
cp .\compile_commands.json ..\compile_commands.json 
