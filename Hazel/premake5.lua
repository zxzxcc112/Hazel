project "Hazel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputDir .. "%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "%{prj.name}")

	pchheader "hzpch.h"
	pchsource "src/hzpch.cpp" --basic for windows

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vender/stb_image/**.h",
		"vender/stb_image/**.cpp",
		"vender/entt/include/**.hpp",
		"vender/ImGuizmo/ImGuizmo.h",
		"vender/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs
	{
		"src",
		"vender/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
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
		"_CRT_SECURE_NO_WARNINGS",
        "YAML_CPP_STATIC_DEFINE"
	}

    filter "files:vender/ImGuizmo/**.cpp"
        flags "NoPCH"

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