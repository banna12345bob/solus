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
IncludeDir["ImGui"] = "Solus/vendor/ImGui"
IncludeDir["glm"] = "Solus/vendor/glm"

group "Dependencies"
	include "Solus/vendor/GLFW"
	include "Solus/vendor/Glad"
	include "Solus/vendor/ImGui"

group ""

project "Solus"
	location "Solus"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("compile/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("compile/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "supch.h"
	pchsource "Solus/src/supch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
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

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SU_PLATFORM_WINDOWS",
			"SU_BUILD_DLL"
		}

filter "system:linux"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_LINUX"
	}

filter "system:macosx"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_MAC"
	}

	filter "configurations:Debug"
		defines "SU_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SU_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SU_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Solus/src",
		"Solus/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Solus"
	}

filter "system:windows"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_WINDOWS"
	}

filter "system:linux"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_LINUX"
	}

filter "system:macosx"
	systemversion "latest"

	defines
	{
		"SU_PLATFORM_MAC"
	}

	filter "configurations:Debug"
		defines "SU_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SU_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SU_DIST"
		runtime "Release"
		optimize "on"
		symbols "on"
		kind "WindowedApp"
		entrypoint "mainCRTStartup"