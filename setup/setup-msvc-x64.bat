REM setup for visual studio environment 64 bit applications
cmake -G "Visual Studio 16 2019" .. "-DVCPKG_TARGET_TRIPLET=x64-windows-static"
cmake --build .
