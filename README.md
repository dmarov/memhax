# MemHax
This repository is a collection of hacks and utils for reverse engineering program memory

- tf-hack.exe - hack for TankForce game (works, F9 - toggle infinity health and ammo, F8 - freeze Z coordinate, needs optimization)
- tf-internal-hack.dll - attempt to make internal hack (work in progress)
- dll-injector.exe - attempt to make dll injector (work in progress)
- ac-hack.exe - hack for Assault Cube (works)
- ac-internal-hack.dll - attempt to make internal hack for Assault Cube (work in progress)
- sigmaker.exe - utility used to generate AOB signatures from multilevel pointers (doesnt't work, not wery useful, will be deleted)
- sigmaker-monitor.exe - same as sigmaker.exe, but works in monitoring mode (doesn't work, will be deleted)

Edit `CMakeLists.txt` in the root to enable or disable build of particular projects (will be improved).

## How to work using any text editor

install msvc 2019 enterprise or community

install ninja build system

```
choco install ninja
```

install package manager for C++

```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```

install dependencies

```
.\vcpkg\vcpkg.exe install boost:x64-windows-static sqlite3:x64-windows-static yaml-cpp:x64-windows-static
```

configure environment

```
mkdir build
cd build
..\configure\ninja-msvc.bat // (or "..\configure\ninja-msvc.bat Release" for release build)
```

build projects

```
cmake --build .
```

now your build directory contains all binaries

execute 

```
cmake --install . --prefix ..
```

to install binaries into `.\bin` directory

## How to work using Visual Studio

install package manager for C++

```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```

install dependencies

```
.\vcpkg\vcpkg.exe install boost:x64-windows-static sqlite3:x64-windows-static yaml-cpp:x64-windows-static
```

configure environment

```
mkdir build
cd build
..\configure\vs-msvc.bat
```

now build directory should contain Visual Studio solution

## Backlog

- improve ac-hack - disable is for opponents
- improve project setup - separate for each project
- optimize signature scan - scan for multiple signature in one scan (signature pool)
- make dll-injector work
- migrate external hacks to internal
- figure out direct3d hooking
- create UI
