# Kernel driver and API library for reading process memory (wip)

## Commands

to stamp inf file

```
stampinf.exe -f .\winkmemdx64.inf  -a "amd64" -k "1.15" -v "*" -d "*"
```

to generate catalog file

```
"C:\Program Files (x86)\Windows Kits\10\bin\x86\inf2cat.exe" /driver:"./" /os:10_X64 /verbose
```

to sign driver with self signed certificate

```
makecert -r -sv ./winkmemdx64.pvk -n CN="MDS" ./winkmemdx64.cer
cert2spc ./winkmemdx64.cer ./winkmemdx64.spc
pvk2pfx -pvk ./winkmemdx64.pvk -spc ./winkmemdx64.spc -pfx ./winkmemdx64.pfx
signtool sign /f ./winkmemdx64.pfx /t http://timestamp.digicert.com /v ./winkmemdx64.cat
```

to check (not necessary)

```
signtool verify /v /pa ./winkmemdx64.cat
```

to enable/disable driver testing mode
as admin (need to do that if will not sign driver with Microsoft approved certificate)

```
bcdedit /set testsigning on
bcdedit /set testsigning off
```

to disable integrity checks (no need to sign driver then)

```
bcdedit /set nointegritychecks on
bcdedit /set nointegritychecks off
```

to install/uninstall driver as service

```
sc create winkmemdx64 binpath="C:\Users\mds3d\repos\memhax\projects\winkmemdx64\build\winkmemdx64.sys" type=kernel
sc start winkmemdx64
sc stop winkmemdx64
sc delete winkmemdx64
```

verify inf file (not nesessary)

```
"C:\Program Files (x86)\Windows Kits\10\Tools\x64\infverif.exe" ./winkmemdx64.inf
```

install driver with inf file
```
C:\Program Files (x86)\Windows Kits\10\Tools\x64\devcon.exe install .\winkmemdx64.inf root\winkmemdx64
```

delete driver with inf file

```
C:\Program Files (x86)\Windows Kits\10\Tools\x64\devcon.exe remove root\winkmemdx64

or

pnputil /delete-driver winkmemdx64.inf /uninstall
```

debug
```
bcdedit /debug on
bcdedit /dbgsettings serial debugport:1 baudrate:115200 /noumex
shutdown -s -t 0
```

set debug messages filter mask
very important for debuging purposed
if not set then no debug output will be visible in WinDBG
also important to build as Debug target, otherwise will be no debug output
```
ed nt!Kd_IHVDRIVER_Mask 0xF
```
