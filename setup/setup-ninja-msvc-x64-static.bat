call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
cmake -G "Ninja" ..  "-DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DVCPKG_TARGET_TRIPLET=x64-windows-static" "-DCMAKE_EXPORT_COMPILE_COMMANDS=1"
cp .\compile_commands.json ..\compile_commands.json 
