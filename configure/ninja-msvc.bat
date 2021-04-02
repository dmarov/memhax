REM examples:
REM cd build
REM ..\configure\ninja-msvc.bat
REM or
REM ..\configure\ninja-msvc.bat Debug
REM or
REM ..\configure\ninja-msvc.bat Debug x86
REM or
REM ..\configure\ninja-msvc.bat Release

set arch=x64
set build_type=Debug
set vars_arch=amd64

if [%1]==[Release] set build_type=%1
if [%2]==[x86] set arch=%2
if [%2]==[x86] set vars_arch=x86_amd64

set vcvars="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
set toolchain="../vcpkg/scripts/buildsystems/vcpkg.cmake"
set triplet="%arch%-windows-static"

call %vcvars% %vars_arch%

cmake -G "Ninja" ..^
 "-DCMAKE_TOOLCHAIN_FILE=%toolchain%"^
 "-DVCPKG_TARGET_TRIPLET=%triplet%"^
 "-DCMAKE_EXPORT_COMPILE_COMMANDS=1"^
 "-DCMAKE_BUILD_TYPE=%build_type%"

cp .\compile_commands.json ..\compile_commands.json