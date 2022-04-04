workspace "Solus"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Solus/vendor/GLFW/include"

include "Solus/vendor/GLFW"

project "Solus"
	location "Solus"
	kind "SharedLib"
	language "C++"

	targetdir ("compile/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("compile/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "supch.h"
	pchsource "Solus/src/supch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links 
	{ 
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SU_PLATFORM_WINDOWS",
			"SU_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../compile/bin/" .. outputdir .. "/Sandbox")
		}

filter "system:linux"
	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_LINUX"
	}

filter "system:macosx"
	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_MAC"
	}

	filter "configurations:Debug"
		defines "SU_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SU_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SU_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("compile/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("compile/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Solus/vendor/spdlog/include",
		"Solus/src"
	}

	links
	{
		"Solus"
	}

filter "system:windows"
	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_WINDOWS"
	}

filter "system:linux"
	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_LINUX"
	}

filter "system:macosx"
	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_MAC"
	}

	filter "configurations:Debug"
		defines "SU_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SU_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SU_DIST"
		optimize "On"