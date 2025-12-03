workspace "Vesper"
	architecture "x64"

	configurations 
	{ 
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Vesper/vendor/GLFW/include"
include "Vesper/vendor/GLFW"

project "Vesper"
	location "Vesper"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vzpch.h"
	pchsource "Vesper/src/vzpch.cpp"

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
		staticruntime "Off"
		systemversion "latest"

    buildoptions { "/utf-8" }


	defines
	{
		"VZ_PLATFORM_WINDOWS", 
		"VZ_BUILD_DLL" 
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
	}

	filter "configurations:Debug"
		defines "VZ_DEBUG"
			symbols "On"

	filter "configurations:Release"
		defines "VZ_RELEASE"
			optimize "On"

	filter "configurations:Dist"
		defines "VZ_DIST"
			optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Vesper/vendor/spdlog/include",
		"Vesper/src"
	}

	links
	{
		"Vesper"
	}

	dependson
    {
        "Vesper"
    }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

    buildoptions { "/utf-8" }


		defines
		{
			"VZ_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "VZ_DEBUG"
				symbols "On"

		filter "configurations:Release"
			defines "VZ_RELEASE"
				optimize "On"

		filter "configurations:Dist"
			defines "VZ_DIST"
				optimize "On"