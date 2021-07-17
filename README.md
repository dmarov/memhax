# MemHax
This repository is a collection of hacks and utils for reverse engineering program memory
source code divided into .\projects\ and .\src

- tf-external-hack.exe - hack for TankForce game (F9 - toggle infinity health and ammo, F8 - freeze Z coordinate, needs optimization, F10 - quit)
- dll-injector.exe - attempt to make dll injector `.\dll-injector\dll-injector.exe --target ac_client.exe --lib "C:\{{PATH}}\ac-internal-hack.dll"`
- ac-external-hack.exe - hack for Assault Cube (F9 - toggle infinity health and ammo, F8 - interrupt)
- ac-internal-hack.dll - internal hack for Assault Cube, inject using aly dll injector (F9 - toggle infinity health and ammo, F10 - quit)

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
    boost:x86-windows-static
```

then refer to specific README in .\projects\{{project_name}}\README.md

## Backlog

- implement kernel mode driver for reading/writing process memory
- implement dll-injector manual mapping
- optimize signature scan - scan for multiple signature in one scan (signature pool)
- improve ac-hack - refactor, fix reltive jump
- optimize dll-injector work
- add dll-injector protection from idiot
- figure out direct3d hooking
- create UI
- make build cross-platform (Ansible)
