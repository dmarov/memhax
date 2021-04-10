# Build

`
cd projects\ac-hack
mkdir build
cd build
..\ninja-msvc.bat Release
cmake --build .
`

now you can run `.\ac-hack.exe`

# Install
`
cmake --install
`

now hack is installed somewhere in `C:\Program Files`

# Run
`
.\ac-hack.exe
.\ac-hack.exe --version
.\ac-hack.exe --help
`
