workspace "Hazel"
	configurations {"Debug", "Release", "Distribution"}
	architecture "x86_64"
	startproject "Hazelnut"

	flags
	{
		"MultiProcessorCompile"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

--Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"]      = "%{wks.location}/Hazel/vender/GLFW/include"
IncludeDir["Glad"]      = "%{wks.location}/Hazel/vender/Glad/include"
IncludeDir["ImGui"]     = "%{wks.location}/Hazel/vender/ImGui"
IncludeDir["glm"]       = "%{wks.location}/Hazel/vender/glm"
IncludeDir["stb_image"] = "%{wks.location}/Hazel/vender/stb_image"
IncludeDir["entt"]      = "%{wks.location}/Hazel/vender/entt/include"
IncludeDir["yaml_cpp"]      = "%{wks.location}/Hazel/vender/yaml-cpp/include"

group "Dependencies"
	include "Hazel/vender/GLFW"
	include "Hazel/vender/Glad"
	include "Hazel/vender/ImGui"
	include "Hazel/vender/yaml-cpp"
group ""

include "Hazel"
include "Hazelnut"
include "Sandbox"