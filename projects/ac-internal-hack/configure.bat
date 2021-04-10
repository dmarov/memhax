set build_type=Debug

if [%1]==[Release] set build_type=%1

..\..\..\scripts\ninja-msvc.bat %build_type% x86 ac-hack
