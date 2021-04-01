REM project setup for mingw32 environment
REM you can only use it in 32 bit environment
cmake -G "MinGW Makefiles" .. "-DVCPKG_TARGET_TRIPLET=x86-mingw-static" "-DVCPKG_APPLOCAL_DEPS=OFF"
