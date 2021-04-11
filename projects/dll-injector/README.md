# Build

```
cd projects\ac-hack
mkdir build
cd build

..\configure.bat Debug
cmake --build .

..\configure.bat Release
cmake --build .
```

now you can run `.\ac-hack.exe`

# Install
```
cmake --install
```

now hack is installed somewhere in `C:\Program Files\bin`

# Run
```
.\dll-injector\dll-injector.exe --target ac_client.exe --lib "C:\{{PATH}}\ac-internal-hack.dll"
```
