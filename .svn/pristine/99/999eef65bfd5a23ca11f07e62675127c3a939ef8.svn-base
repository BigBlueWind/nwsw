@ECHO OFF

::
:: Check if this file properly called
::
IF "%MAKETOOL%" == "" GOTO error

:loop
IF "%1" == "" GOTO :EOF

::
:: Copy files procedure
::
IF /i "%1" == "copyfiles" (
%MAKETOOL% copyfiles
)

SHIFT
GOTO loop

:error
ECHO Do not call this batch directly
