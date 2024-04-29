CALL .\restore_orig.bat
CALL .\copy_orig.bat
IF errorlevel 1 (
ECHO Exiting premake...
PAUSE
EXIT /b 1
)
pushd ..\..\
CALL .\vender\premake\premake5.exe vs2022
popd
PAUSE