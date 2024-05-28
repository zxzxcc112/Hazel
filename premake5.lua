workspace "Hazel"
	configurations {"Debug", "Release", "Distribution"}
	architecture "x86_64"
	startproject "Sandbox"

	flags
	{
		"MultiProcessorCompile"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

--Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vender/GLFW/include"
IncludeDir["Glad"] = "Hazel/vender/Glad/include"
IncludeDir["ImGui"] = "Hazel/vender/ImGui"
IncludeDir["glm"] = "Hazel/vender/glm"
IncludeDir["stb_image"] = "Hazel/vender/stb_image"

group "Dependencies"
	include "Hazel/vender/GLFW"
	include "Hazel/vender/Glad"
	include "Hazel/vender/ImGui"
group ""

project "Hazel"
	location "Hazel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputDir .. "%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "%{prj.name}")

	pchheader "hzpch.h"
	pchsource "%{prj.name}/src/hzpch.cpp" --basic for windows

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vender/stb_image/**.h",
		"%{prj.name}/vender/stb_image/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vender/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	disablewarnings
	{
		"4819"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Hazel/vender/ImGui",
		"Hazel/src"
	}

	links
	{
		"Hazel"
	}

	disablewarnings
	{
		"4819"
	}
	
	defines
	{
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

project "Hazelnut"
	location "Hazelnut"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Hazel/vender/ImGui",
		"Hazel/src"
	}

	links
	{
		"Hazel"
	}

	disablewarnings
	{
		"4819"
	}
	
	defines
	{
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"