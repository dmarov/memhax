rmdir /s /q .\build
mkdir build
cd build
CALL ..\configure.bat Release
cmake --build .
