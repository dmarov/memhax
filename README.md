# MemHax
This repository is a collection of hacks and utils for reverse engineering program memory

- sigmaker.exe - utility used to generate AOB signatures from multilevel pointers
- sigmaker-monitor.exe - same as sigmaker.exe, but works in monitoring mode (will be deleted)
- tf-hack.exe - hack for TankForce game

## How to build

```
// install msvc 2019
// install ninja build system
choco install ninja
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg.exe install boost:x64-windows-static sqlite3:x64-windows-static yaml-cpp:x64-windows-static
mkdir build
cd build
..\setup\build-ninja-msvc-x64-static-release.bat
cmake --build .
cmake --install . --prefix ..
```
now in ./bin directory should be all executables
