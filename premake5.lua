workspace "Solus"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Solus"
	location "Solus"
	kind "SharedLib"
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
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
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

	filter "configurations:Debug"
		defines "SU_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SU_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SU_DIST"
		optimize "On"