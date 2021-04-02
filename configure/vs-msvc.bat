REM examples:
REM cd build
REM ..\configure\vs-msvc.bat
REM or
REM ..\configure\vs-msvc.bat Debug
REM or
REM ..\configure\vs-msvc.bat Debug x86
REM or
REM ..\configure\vs-msvc.bat Release

set arch=x64
set build_type=Debug
set vars_arch=amd64
set bit_flag=""

if [%1]==[Release] set build_type=%1
if [%2]==[x86] set arch=%2
if [%2]==[x86] set vars_arch=x86_amd64
if [%arch%]==[x86] set bit_flag=-A Win32

set triplet="%arch%-windows-static"
set vcvars="C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
set toolchain="../vcpkg/scripts/buildsystems/vcpkg.cmake"

call %vcvars% %vars_arch%

cmake -G "Visual Studio 16 2019" %bit_flag% ..^
 "-DCMAKE_TOOLCHAIN_FILE=%toolchain%"^
 "-DVCPKG_TARGET_TRIPLET=%triplet%"^
 "-DCMAKE_BUILD_TYPE=%build_type%"
