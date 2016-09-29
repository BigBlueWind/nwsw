@echo on

cd ../..
set PRJDIR=%CD%
set PATH=%PRJDIR%/Bin;%PATH%
cd T4KDev/LTERT
 
make -C %PRJDIR% -s -f T4KDev/LTERT/makefile-larm %* "SHELL=cmd.exe"

