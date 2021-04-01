REM setup for visual studio environment 32 bit applications
cmake -G "Visual Studio 16 2019" -A Win32  .. "-DVCPKG_TARGET_TRIPLET=x86-windows-static"
cmake --build .
