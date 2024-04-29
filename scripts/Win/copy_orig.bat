::Copy original vs2022 build project

SET PROJECTPATH=..\..\

SET DESTINATION=.\doc\orig\

IF EXIST %PROJECTPATH%%DESTINATION%\Hazel.sln (
ECHO Original build projects is exist. Exiting...
PAUSE
EXIT /b 2
)


SET vs2022Build=.\Hazel.sln .\Hazel\Hazel.vcxproj .\Hazel\Hazel.vcxproj.filters .\Hazel\Hazel.vcxproj.user ^
                .\Sandbox\Sandbox.vcxproj .\Sandbox\Sandbox.vcxproj.filters .\Sandbox\Sandbox.vcxproj.user ^
		.\Hazel\vender\GLFW\GLFW.vcxproj .\Hazel\vender\GLFW\GLFW.vcxproj.filters  .\Hazel\vender\GLFW\GLFW.vcxproj.user ^
		.\Hazel\vender\Glad\Glad.vcxproj .\Hazel\vender\Glad\Glad.vcxproj.filters  .\Hazel\vender\Glad\Glad.vcxproj.user ^
		.\Hazel\vender\ImGui\ImGui.vcxproj .\Hazel\vender\ImGui\ImGui.vcxproj.filters  .\Hazel\vender\ImGui\ImGui.vcxproj.user

FOR %%I IN (%vs2022Build%) DO XCOPY /Y %PROJECTPATH%%%I %PROJECTPATH%%DESTINATION%

PAUSE