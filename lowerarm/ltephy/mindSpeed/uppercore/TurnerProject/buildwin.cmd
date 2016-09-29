@ECHO OFF

SETLOCAL

SET FOURG=WiMAX
SET MACTYPE=STUB
SET DBG=Debug

:loop
IF "%1" == "" GOTO start

IF "%1" == "clean" (
	SET TARGET=/clean
	GOTO start
)

IF "%1" == "msmac" (
	SET FOURG=WiMAX
	SET MACTYPE=MSMAC
	IF "%2" == "release" (SET DBG=Release)
	GOTO start
)

IF "%1" == "msmacdll" (
	SET FOURG=WiMAX
	SET MACTYPE=MSMAC
	SET DBG=Debug-DLL
	IF "%2" == "release" (SET DBG=Release-DLL)
	GOTO start
)

IF "%1" == "msmacusephydll" (
	SET FOURG=WiMAX
	SET MACTYPE=MSMACforPhyCdll
	GOTO start
)

IF "%1" == "bsmac" (
	SET FOURG=WiMAX
	SET MACTYPE=MSMAC
	IF "%2" == "release" (SET DBG=Release)
	GOTO start
)

IF "%1" == "enodeb" (
	SET FOURG=LTE
	SET MACTYPE=eNodeB
	IF "%2" == "release" (SET DBG=Release)
	GOTO start
)

IF "%1" == "enodebdll" (
	SET FOURG=LTE
	SET MACTYPE=eNodeB
	SET DBG=DLL-Debug
	GOTO start
)

IF "%1" == "ue" (
	SET FOURG=LTE
	SET MACTYPE=UE
	IF "%2" == "release" (SET DBG=Release)
	GOTO start
)

IF "%1" == "uedll" (
	SET FOURG=LTE
	SET MACTYPE=UE
	SET DBG=Debug-DLL
	IF "%2" == "release" (SET DBG=Release-DLL)
	GOTO start
)

SHIFT
GOTO loop

:start
IF NOT "%TARGET%" == "/clean" SET TARGET=%FOURG%-%MACTYPE%-%DBG%
IF NOT "%vs90comntools%" == "" GOTO vs2008
IF NOT "%vs80comntools%" == "" GOTO vs2005
IF "%comntools%" == "" GOTO no_toolchain
GOTO vs60

:vs2008
CALL "%vs90comntools%"\vsvars32.bat
vcbuild TurnerProject.vcproj %TARGET%
GOTO end

:vs2005
GOTO end

:vs60
GOTO end

:no_toolchain
ECHO No tool chain detected
GOTO end

:end
ENDLOCAL

