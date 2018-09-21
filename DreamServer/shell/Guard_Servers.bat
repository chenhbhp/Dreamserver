
@ECHO OFF

ECHO 		The script is used to restart EXEs when they dump

SET TIME_INTERVAL=3
SET SERVERS=(AServer.exe MServer.exe GServer.exe)

:loop

rem tasklist > tasktmp.txt
rem rd tasktmp.txt

::TASKLIST /nh | FIND /i "AServer.exe"
REM check exist EXEs
::IF ERRORLEVEL 1 (START AServer.exe) ELSE (ECHO Running AServer.exe)

::NUL setting >NUL means print null
FOR %%S IN %SERVERS% do (
TASKLIST /nh | FIND /i "%%S" > NUL
IF ERRORLEVEL 1 (START %%S) ELSE (ECHO Running %%S)
) 

REM timeout -t %TIME_INTERVAL% -nobreak > nul

ping -n %TIME_INTERVAL% 127.0.0.1 > NUL

GOTO loop

REM kill process
REM taskkill -im AServer.exe or AServer*

REM pause