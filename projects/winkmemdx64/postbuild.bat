set drvname=winkmemdx64
set inf2cat_path="C:\Program Files (x86)\Windows Kits\10\bin\x86\inf2cat.exe"

:: replace variables in inf file
stampinf.exe -f .\%drvname%.inf  -a "amd64" -k "1.15" -v "*" -d "*"
:: generate catalog file
%inf2cat_path% /driver:"./" /os:10_X64 /verbose
:: creating untrusted root certificate
makecert -r -sv ./%drvname%.pvk -n CN="MDS" ./%drvname%.cer
:: creating software publisher certificate from certificate
cert2spc ./%drvname%.cer ./%drvname%.spc
:: copy pubk and privk info to Persolan Information Exchange
pvk2pfx -f -pvk ./%drvname%.pvk -spc ./%drvname%.spc -pfx ./%drvname%.pfx
:: sign cat file
signtool sign -f ./%drvname%.pfx -t http://timestamp.digicert.com -v ./%drvname%.cat

::openssl genrsa -out %drvname%.key 2048
::openssl pkcs12 -export -name "MDS" -out %drvname%.pfx -inkey %drvname%.key -in %drvname%.crt
