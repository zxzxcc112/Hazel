::Restore original vs2022 build projects

SET ProjectPath=..\..\
SET Solution=Hazel.sln
SET Projects=Hazelnut Hazel Sandbox
SET orig_path=.\doc\orig\

IF NOT EXIST %ProjectPath%%orig_path%%Solution% (
    ECHO No file can restore...
    PAUSE
    EXIT /b 1
)

MOVE %ProjectPath%%orig_path%%Solution% %ProjectPath%.\
FOR %%I IN (%Projects%) DO MOVE %ProjectPath%%orig_path%%%I* %ProjectPath%.\%%I\


SET SubProjects=GLFW Glad ImGui yaml-cpp
FOR %%I IN (%SubProjects%) DO MOVE %ProjectPath%%orig_path%%%I* %ProjectPath%.\Hazel\vender\%%I\

PAUSE  
