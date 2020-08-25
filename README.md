# MemHax

.\sigmaker.exe --config config.yml

## Build

```
choco install ninja
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg.exe install boost:x64-windows-static sqlite3:x64-windows-static yaml-cpp:x64-windows-static
mkdir build
cd build
..\setup\build-ninja-msvc-x64-static.bat
cmake --build .
```
