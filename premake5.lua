workspace "Solus"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Solus/vendor/GLFW/include"
IncludeDir["Glad"] = "Solus/vendor/Glad/include"

group "Dependencies"
	include "Solus/vendor/GLFW"
	include "Solus/vendor/Glad"

group ""

project "Solus"
	location "Solus"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"SU_PLATFORM_WINDOWS",
			"SU_BUILD_DLL",
			"SU_ENABLE_ASSERTS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../compile/bin/" .. outputdir .. "/Sandbox/\"")
		}

filter "system:linux"
	cppdialect "C++17"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_LINUX"
	}

filter "system:macosx"
	cppdialect "C++17"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_MAC"
	}

	filter "configurations:Debug"
		defines "SU_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SU_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "SU_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_WINDOWS",
		"SU_ENABLE_ASSERTS"
	}

filter "system:linux"
	cppdialect "C++17"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_LINUX"
	}

filter "system:macosx"
	cppdialect "C++17"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_MAC"
	}

	filter "configurations:Debug"
		defines "SU_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SU_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "SU_DIST"
		runtime "Release"
		optimize "On"