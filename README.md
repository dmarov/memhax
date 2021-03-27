# MemHax
This repository is a collection of hacks and utils for reverse engineering program memory

- tf-hack.exe - hack for TankForce game (works, F9 - toggle infinity health and ammo, F8 - freeze Z coordinate)
- tf-inlernal-hack.dll - attempt to make internal hack (work in progress)
- dll-injector.exe - attempt to make dll injector (work in progress)
- ac-hack.dll - hack for Assault Cube (works)
- ac-inlernal-hack.dll - attempt to make internal hack for Assault Cube (work in progress)
- sigmaker.exe - utility used to generate AOB signatures from multilevel pointers (doesnt't work, not wery useful, wil be deleted)
- sigmaker-monitor.exe - same as sigmaker.exe, but works in monitoring mode (doesn't work, will be deleted)


## How to work without IDE

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

setup environment

```
mkdir build
cd build
..\setup\build-ninja-msvc-x64-static.bat // (or ..\setup\build-ninja-msvc-x64-static-release.bat fro release build)
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

## How to work with Visual Studio


install package manager for C++

```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```

install dependencies

```
.\vcpkg\vcpkg.exe install boost:x64-windows-static sqlite3:x64-windows-static yaml-cpp:x64-windows-static
```

setup environment

```
mkdir build
cd build
..\setup\build-msvc-x64-static.bat
```

now build directory should contain Visual Studio solution
