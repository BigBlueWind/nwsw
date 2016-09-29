@ECHO OFF

set BIN=rb_acc.elf
set LTELIB=libltel1mspd.a
rem set PHYLIB=libltel1.a
set TESTAPPLIB=
rem set LTERT=C:\CCPU\rkks\SSI_Latest\Master_MSPD_CCPU\ltephy\mindSpeed\lowercore\T4KDev\LTERT
set LTERT=C:\ssi_latest\Master_MSPD_CCPU\ltephy\mindSpeed\lowercore\T4KDev\LTERT
rem set LTERT=C:\CCPU\SSI\SSI_new\lowercore\T4KDev\LTERT
set CORELIB=%LTERT%\..\BIN\SYSCORE_D.a
set MXLIB=%LTERT%\..\BIN\4GMXLIB_D.a

set GEN_DEP=
set MMUFLAG="-DMX_ARM_MMU_ENABLED"

set LWR_ARM_DIR=%CD%
set WIMAXDIR=%LTERT%\..\..\PHY\WiMAX\C-Code\prj\LowerArmApp
set SYSCOREDIR=%LTERT%\..\SYSCORE
set OS4GMXDIR=%LTERT%\..\4GMX 
set LTEDIR=%LTERT%\..\..\PHY\LTE\Arm\Prj\LtePhyRt
set TESTAPPDIR=%LTERT%\..\..\..\..\..\lteenb\build\


set RELEASECFG="-DMX_ARM_ENABLED" "-DLOWERARM" "-DEXECUTION_FROM_TASK_LIST" "-DUSECEVA" "-DWIMAX" 
set DEBUGCFG=
set RELEASEBUILD=0

@del *.elf*
@del *.lst
@del *.o
@del *.err

rem if /i "%3" == "release" set RELEASEBUILD=1

set RELEASEBUILD=1

if %RELEASEBUILD% == 0 GOTO DEBUG_BUILD

:RELEASE_BUILD
set CORELIB=%LTERT%\..\BIN\SYSCORE_R.a
set MXLIB=%LTERT%\..\BIN\4GMXLIB_R.a
set OS4GMX_BUILD=make -f makefile release
set SYSCORE_BUILD=make -f makefile release

GOTO BUILD_CFG

:DEBUG_BUILD
set DEBUGCFG="-D_MX_DEBUG" "-D_APP_ASSERT_HANDLER"
set OS4GMX_BUILD=make -f makefile debug
set SYSCORE_BUILD=make -f makefile debug

:BUILD_CFG
set BUILDCFG=%RELEASECFG%%DEBUGCFG% 

rem if /i NOT "%1" == "dep" goto NO_GENERATE_DEP

set GEN_DEP=1

rem :NO_GENERATE_DEP
rem if /i "%2" == "lte" goto BUILD_LTE

goto BUILD_LTE

:BUILD_LTE
rem if /i "%1" == "nocore" goto BUILD_PHY
echo.
echo ===================================
echo 4GMX building ...
echo ===================================

cd %OS4GMXDIR%
make clean
call %OS4GMX_BUILD%

if errorlevel 1 goto BUILDERROR

if exist .\4GMXLIB_D.a xcopy /Y /D /F /R .\4GMXLIB_D.a ..\BIN
if exist .\4GMXLIB_R.a xcopy /Y /D /F /R .\4GMXLIB_R.a ..\BIN

cd %LWR_ARM_DIR%

echo.
echo ===================================
echo SysCore building ...
echo ===================================

cd %SYSCOREDIR%
make clean
call %SYSCORE_BUILD%

if errorlevel 1 goto BUILDERROR

if exist .\SYSCORE_D.a xcopy /Y /D /F /R .\SYSCORE_D.a ..\BIN
if exist .\SYSCORE_R.a xcopy /Y /D /F /R .\SYSCORE_R.a ..\BIN

cd %LWR_ARM_DIR%

:BUILD_PHY
echo.
echo ===================================
echo LTE PHY building ...
echo ===================================

rem THIS IS FOR FDX
set RELEASECFG="-DMX_ARM_ENABLED" "-DLOWERARM" "-DSCHEDULER" "-DUL_CEVA" "-DUSECEVA" "-DRT_API_PROC" "-DUL_STATIC_LIST" "-DFDX_RT" "-DLTE_PHY_RT" "-DSDU_THREAD" "-DUSEFEC" "-DLTE" "-DPHY_API_PROTECTION" "-DLIST_BASED" 
set DEBUGCFG=
if NOT %RELEASEBUILD% == 1 set DEBUGCFG="-D_MX_DEBUG" "-D_APP_ASSERT_HANDLER" 

set BUILDCFG=%RELEASECFG%%DEBUGCFG% 

set BIN=testarm.elf
set PHYLIB=%LTELIB%

cd %LTEDIR%
REM Build lower layer of PHY library (visible as OBJ)
IF EXIST makefile-low make -f makefile-low "LTELIB=%LTELIB%" "PHYLIB=%PHYLIB%" "BUILDCFG=%BUILDCFG%" "GEN_DEP=%GEN_DEP%" "LWR_ARM_DIR=%LWR_ARM_DIR%"
IF NOT EXIST makefile-low SET LTELIB=..\BIN\libltel1mspd.a
if errorlevel 1 goto BUILDERROR

REM Build upper layer of PHY library (visible as SRC)
IF EXIST makefile-lib make -f makefile-lib "LTELIB=%LTELIB%" "PHYLIB=%PHYLIB%" "BUILDCFG=%BUILDCFG%" "GEN_DEP=%GEN_DEP%" "LWR_ARM_DIR=%LWR_ARM_DIR%"
if errorlevel 1 goto BUILDERROR

cd %LWR_ARM_DIR%
rem set PHYLIB=libltel1.a
IF EXIST %LTEDIR%\%LTELIB% copy /y %LTEDIR%\%LTELIB% .
rem IF EXIST %LTEDIR%\%PHYLIB% copy /y %LTEDIR%\%PHYLIB% .

cd %LWR_ARM_DIR%

echo.
echo ===================================
echo Test App building ...
echo ===================================

cd %TESTAPPDIR%
set MACLIB=%TESTAPPLIB%
set RELEASECFG="-DMX_ARM_ENABLED" "-DLOWERARM" "-DENODEB" "-DFDX_RT" "-DCCPU_4GMX_PORT" 

set DEBUGCFG=
if NOT %RELEASEBUILD% == 1 set DEBUGCFG="-D_MX_DEBUG" "-D_APP_ASSERT_HANDLER" 

set BUILDCFG=%RELEASECFG%%DEBUGCFG% 

set BIN=rb_acc.elf
set MACLIB=%TESTAPPLIB%

cd %TESTAPPDIR%
rem make all
rem if errorlevel 1 goto BUILDERROR

cd %LWR_ARM_DIR%
rem IF EXIST %TESTAPPDIR%\%TESTAPPLIB% copy /y %TESTAPPDIR%\%TESTAPPLIB% .

:APP_BLD
echo.
echo ===================================
echo Application building ...
echo ===================================
nmake /NOLOGO /E /S -f "Makefile-ssi" %BIN% "BIN=%BIN%" "SCT=prj.sct" "CORELIB=%CORELIB%" "MXLIB=%MXLIB%" "PHYLIB=%PHYLIB%" "LTELIB=%LTELIB%" "MACLIB=%MACLIB%" "BUILDCFG=%BUILDCFG%" "GEN_DEP=%GEN_DEP%" "LTERT=%LTERT%"
if errorlevel 1 goto BUILDERROR
if "%4" == "yes" (
pscp -pw root123 *.elf root@10.1.35.163:/opt/t4xxx/bsp-0.1.7.2/var/lib/tftpboot/59
)

@fromelf --text -c %BIN% > %BIN%.asm
@fromelf --text -d %BIN% > %BIN%.hex

:BUILDERROR
cd %LWR_ARM_DIR%
