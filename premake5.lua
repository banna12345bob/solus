workspace "Solus"
	architecture "x86_64"
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
IncludeDir["stb_image"] = "Solus/vendor/stb_image"
IncludeDir["libnyquist"] = "Solus/vendor/libnyquist/include"
IncludeDir["rtaudio"] = "Solus/vendor/libnyquist/third_party/rtaudio"

group "Dependencies"
	include "Solus/vendor/GLFW"
	include "Solus/vendor/Glad"
	include "Solus/vendor/ImGui"
	include "Solus/vendor/libnyquist"
	include "Solus/vendor/libnyquist/third_party/rtaudio"

group ""

project "Solus"
	location "Solus"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("compile/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("compile/bin-int/" .. outputdir .. "/%{prj.name}")

	-- pchheader "supch.h"
	-- pchsource "Solus/src/supch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{IncludeDir.stb_image}/**.h",
		"%{IncludeDir.stb_image}/**.cpp",
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.libnyquist}",
		"%{IncludeDir.rtaudio}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"libnyquist",
		"rtAudio",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		pchheader "supch.h"
		pchsource "Solus/src/supch.cpp"

	filter "system:linux"
		systemversion "latest"

	filter "system:macosx"
		systemversion "latest"
        xcodebuildsettings { ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }
		pchheader "src/supch.h"
		pchsource "Solus/src/supch.cpp"

	filter "configurations:Debug"
		defines "SU_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SU_RELEASE"
		runtime "Release"
		symbols "off"
		optimize "on"

	filter "configurations:Dist"
		defines "SU_DIST"
		runtime "Release"
		symbols "off"
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
		"%{IncludeDir.libnyquist}",
		"%{IncludeDir.rtaudio}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Solus"
	}

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		systemversion "latest"

	filter "system:macosx"
		systemversion "latest"
        xcodebuildsettings { ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }

	filter "configurations:Debug"
		defines "SU_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SU_RELEASE"
		runtime "Release"
		symbols "off"
		optimize "on"

	filter "configurations:Dist"
		defines "SU_DIST"
		runtime "Release"
		optimize "on"
		symbols "off"
		kind "WindowedApp"
		entrypoint "mainCRTStartup"
