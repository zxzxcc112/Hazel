workspace "Hazel"
	configurations {"Debug", "Release", "Distribution"}
	architecture "x64"
	startproject "Sandbox"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

--Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vender/GLFW/include"
IncludeDir["Glad"] = "Hazel/vender/Glad/include"
IncludeDir["ImGui"] = "Hazel/vender/ImGui"
IncludeDir["glm"] = "Hazel/vender/glm"

include "Hazel/vender/GLFW"
include "Hazel/vender/Glad"
include "Hazel/vender/ImGui"

project "Hazel"
	location "Hazel"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputDir .. "%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "%{prj.name}")

	pchheader "hzpch.h"
	pchsource "%{prj.name}/src/hzpch.cpp" --basic for windows

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vender/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	postbuildcommands 
	{
		("{mkdir} ../bin/" .. outputDir .. "Sandbox"),
		("{copyfile} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "Sandbox")
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "HZ_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputDir .. "%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Hazel/vender/spdlog/include",
		"Hazel/vender/glm",
		"Hazel/src"
	}

	links
	{
		"Hazel"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "HZ_DIST"
		runtime "Release"
		optimize "On"