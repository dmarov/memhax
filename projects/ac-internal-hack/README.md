# Build

```
cd projects\ac-internal-hack
mkdir build
cd build

..\configure.bat Debug
cmake --build .

..\configure.bat Release
cmake --build .
```

# Install
```
cmake --install
```

now hack is installed somewhere in `C:\Program Files\lib`

# Inject
```
dll-injector --target="ac_client.exe" --lib="C:\{{PATH}}\ac-internal-hack.dll"
```
