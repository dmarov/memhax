rmdir /s /q .\build
mkdir build
cd build
CALL ..\configure.bat Debug
cmake --build .
..\postbuild.bat
