project "Hazelnut"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputDir .. "%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Hazel/src",
		"%{wks.location}/Hazel/vender/spdlog/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
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