# MemHax

.\memhax.exe --config config.yml

config.yml

```
addreses
    - id: health
      type: signature
      value: A1 A1 A1 A1 E1 C1 B1 00 B1 B1 B1 B1
      mask:  ?  ?  ?  ?  x  x  x  x  ?  ?  ?  ?
      data_type: integer
    - id: position
      type: multipointer
      offsets: "ac_client.exe" 0x003231 0xD4 0x0 0xFFF
      data_type: 3dvector_float
```

## Build

```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg.exe install boost:x64-windows-static sqlite3:x64-windows-static yaml-cpp:x64-windows-static
mkdir build
cd build
..\setup\build-ninja-msvc-x64-static.bat
```
