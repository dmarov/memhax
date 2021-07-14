Stapminf
Inf2Cat
makecert -r -sv ./winkmemdx64.pvk -n CN="MDS" ./winkmemdx64.cer

cert2spc ./winkmemdx64.cer ./winkmemdx64.spc

pvk2pfx -pvk ./winkmemdx64.pvk -spc ./winkmemdx64.spc -pfx ./winkmemdx64.pfx

"C:\Program Files (x86)\Windows Kits\10\bin\x86\inf2cat.exe" /driver:"./" /os:10_X64 /verbose
