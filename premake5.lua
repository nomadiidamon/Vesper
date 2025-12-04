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
IncludeDir["Glad"] = "Vesper/vendor/Glad/include"
IncludeDir["ImGui"] = "Vesper/vendor/imgui"

include "Vesper/vendor/GLFW"
include "Vesper/vendor/Glad"
include "Vesper/vendor/imgui"

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
		"%{prj.name}/src/**.cpp",
		"Vesper/vendor/imgui/backends/imgui_impl_glfw.cpp",
		"Vesper/vendor/imgui/backends/imgui_impl_opengl3.cpp",
	}

	filter "files:Vesper/vendor/imgui/backends/**.cpp"
		flags { "NoPCH" }
	filter {}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
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
		"VZ_BUILD_DLL",
		"GLFW_INCLUDE_NONE",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
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