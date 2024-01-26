::Restore original vs2022 build projects

SET Solution=Hazel.sln
SET Projects=Hazel Sandbox
SET orig_path=.\doc\orig\

IF NOT EXIST %orig_path%%Solution% (
    ECHO No file can restore...
    PAUSE
    EXIT /b 1
)

MOVE %orig_path%%Solution% .\
FOR %%I IN (%Projects%) DO MOVE %orig_path%%%I* .\%%I\


SET SubProjects=GLFW
FOR %%I IN (%SubProjects%) DO MOVE %orig_path%%%I* .\Hazel\vender\%%I\

PAUSE  
