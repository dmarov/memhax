# Commands

generate cat file

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

to check

```

signtool verify /v /pa ./winkmemdx64.cat

```

to enable/disable driver testing mode
as admin

```

bcdedit /set testsigning on
bcdedit /set testsigning off

```

to disable integrity checks

```

bcdedit /set nointegritychecks on
bcdedit /set nointegritychecks off

```

to install/uninstall driver

```

sc create winkmemdx64 binpath="C:\Users\mds3d\repos\memhax\projects\winkmemdx64\build\winkmemdx64.sys" type=kernel
sc start winkmemdx64
sc stop winkmemdx64
sc delete winkmemdx64

```

verify inf file

```

"C:\Program Files (x86)\Windows Kits\10\Tools\x64\infverif.exe" ./winkmemdx64.inf

```
