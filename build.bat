@ECHO OFF

REM Path to g++
SET G="g++"

SET OBJECTS=
REM Recursive every .cpp file in ./src
FOR /R "./src" %%a IN (*.c*) DO (
    CALL SET OBJECTS=%%OBJECTS%% "%%a"
)

@ECHO ON

%G% %OBJECTS% -obin/a.exe -Iinclude -Llib -lglfw3 -lopengl32 -lgdi32

pause