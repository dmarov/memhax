# MemHax
This repository is a collection of hacks and utils for reverse engineering program memory
source code divided into .\projects\ and .\src 

- tf-hack.exe - hack for TankForce game (works, F9 - toggle infinity health and ammo, F8 - freeze Z coordinate, needs optimization)
- tf-internal-hack.dll - attempt to make internal hack (work in progress)
- dll-injector.exe - attempt to make dll injector (work in progress)
- ac-hack.exe - hack for Assault Cube (works)
- ac-internal-hack.dll - attempt to make internal hack for Assault Cube (work in progress)

## How to work using any text editor

install msvc 2019 enterprise or community

install ninja build system

```
choco install ninja
```

install package manager for C++

```
cd memhax
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```

install dependencies

```
.\vcpkg\vcpkg.exe install
    boost:x64-windows-static
    sqlite3:x64-windows-static
    yaml-cpp:x64-windows-static
    boost:x86-windows-static
    sqlite3:x86-windows-static
    yaml-cpp:x86-windows-static
```

then refer to specific README in .\projects\{{project_name}}\README.md

## Backlog

- improve ac-hack - refactor
- make dll-injector work
- optimize signature scan - scan for multiple signature in one scan (signature pool)
- migrate external hacks to internal
- figure out direct3d hooking
- create UI
