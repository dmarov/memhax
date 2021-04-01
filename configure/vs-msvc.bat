REM setup for visual studio environment 64 bit applications
cmake --build .

REM examples:
REM cd build
REM ..\configure\msvc.bat
REM or
REM ..\configure\msvc.bat Debug
REM or
REM ..\configure\msvc.bat Debug x86
REM or
REM ..\configure\msvc.bat Release


set arch="x64"
set vcvars="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
set toolchain="../vcpkg/scripts/buildsystems/vcpkg.cmake"
set build_type="Debug"

if [%1]==[] goto nospec_build_type
set build_type=%1
:nospec_build_type

if [%2]==["x86"] goto nospec_arch
set arch=%2
:nospec_arch

set triplet="%arch%-windows-static"
call %vcvars%

set bit_flag=""

if [%arch%]==["x86"] set bit_falg="-A Win32"

cmake -G "Visual Studio 16 2019" %bit_flag% ..^
 "-DCMAKE_TOOLCHAIN_FILE=%toolchain%"^
 "-DVCPKG_TARGET_TRIPLET=%triplet%"^
 "-DCMAKE_EXPORT_COMPILE_COMMANDS=1"^
 "-DCMAKE_BUILD_TYPE=%build_type%"
