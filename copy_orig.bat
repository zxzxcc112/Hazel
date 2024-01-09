::Copy original vs2022 build project

IF EXIST .\doc\orig\Hazel.sln (
ECHO Original build projects is exist. Exiting...
PAUSE
EXIT /b 2
)

SET DESTINATION=.\doc\orig\

SET vs2022Build=.\Hazel.sln .\Hazel\Hazel.vcxproj .\Hazel\Hazel.vcxproj.filters .\Hazel\Hazel.vcxproj.user ^
                .\Sandbox\Sandbox.vcxproj .\Sandbox\Sandbox.vcxproj.filters .\Sandbox\Sandbox.vcxproj.user

FOR %%I IN (%vs2022Build%) DO XCOPY /Y %%I %DESTINATION%

PAUSE