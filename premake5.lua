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
IncludeDir["stb_image"] = "Solus/vendor/stb_image"
IncludeDir["msdfgen"] = "Solus/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "Solus/vendor/msdf-atlas-gen/msdf-atlas-gen"

group "Dependencies"
	include "Solus/vendor/GLFW"
	include "Solus/vendor/Glad"
	include "Solus/vendor/ImGui"
	include "Solus/vendor/msdf-atlas-gen"

group ""

project "Solus"
	location "Solus"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("compile/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("compile/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "supch.h"
	pchsource "Solus/src/supch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{IncludeDir.stb_image}/**.h",
		"%{IncludeDir.stb_image}/**.cpp",
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.msdf_atlas_gen}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"msdf-atlas-gen",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		systemversion "latest"

	filter "system:macosx"
		systemversion "latest"

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
		symbols "off"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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

	filter "system:linux"
		systemversion "latest"

	filter "system:macosx"
		systemversion "latest"

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
		symbols "off"
		kind "WindowedApp"
		entrypoint "mainCRTStartup"