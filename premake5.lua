workspace "Vesper"
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
IncludeDir["GLFW"] = "Vesper/vendor/GLFW/include"
IncludeDir["Glad"] = "Vesper/vendor/Glad/include"
IncludeDir["ImGui"] = "Vesper/vendor/imgui"
IncludeDir["glm"] = "Vesper/vendor/glm"
IncludeDir["stb_image"] = "Vesper/vendor/stb_image"

group "Dependencies"
	include "Vesper/vendor/GLFW"
	include "Vesper/vendor/Glad"
	include "Vesper/vendor/imgui"

group ""

project "Vesper"
	location "Vesper"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vzpch.h"
	pchsource "Vesper/src/vzpch.cpp"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"Vesper/vendor/glm/glm/**.hpp",
		"Vesper/vendor/glm/glm/**.inl"
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
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
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

    buildoptions { "/utf-8" }


	defines
	{
		"VZ_PLATFORM_WINDOWS", 
		"GLFW_INCLUDE_NONE",
		"VZ_BUILD_DLL",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "configurations:Debug"
		defines "VZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "VZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "VZ_DIST"
		runtime "Release"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"Vesper/src",
		"Vesper/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Vesper"
	}

	filter "system:windows"
		systemversion "latest"

    buildoptions { "/utf-8" }


		defines
		{
			"VZ_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "VZ_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "VZ_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "VZ_DIST"
			runtime "Release"
			optimize "on"