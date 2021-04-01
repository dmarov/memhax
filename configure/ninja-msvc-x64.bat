set vcvars="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
set toolchain="../vcpkg/scripts/buildsystems/vcpkg.cmake"
set triplet="x64-windows-static"
set build_type="Debug"

if [%1]==[] goto nospec_build_type
set build_type=%1
:nospec_build_type

call %vcvars%

cmake -G "Ninja" ..^
 "-DCMAKE_TOOLCHAIN_FILE=%toolchain%"^
 "-DVCPKG_TARGET_TRIPLET=%triplet%"^
 "-DCMAKE_EXPORT_COMPILE_COMMANDS=1"^
 "-DCMAKE_BUILD_TYPE=%build_type%"

cp .\compile_commands.json ..\compile_commands.json
