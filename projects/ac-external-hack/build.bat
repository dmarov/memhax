rm -rdf build
mkdir build
cd build
CALL ..\configure.bat Release
cmake --build .
