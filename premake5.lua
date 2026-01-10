workspace "Vesper"
	architecture "x64"

	startproject "Vesper-Editor"

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
IncludeDir["entt"] = "Vesper/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Vesper/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Vesper/vendor/ImGuizmo"

group "Dependencies"
	include "Vesper/vendor/GLFW"
	include "Vesper/vendor/Glad"
	include "Vesper/vendor/imgui"
	include "Vesper/vendor/yaml-cpp"

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
		"Vesper/vendor/glm/glm/**.inl",

		"Vesper/vendor/ImGuizmo/ImGuizmo.h",
		"Vesper/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	filter "files:Vesper/vendor/imgui/backends/**.cpp"
		flags { "NoPCH" }
	filter "files:Vesper/vendor/ImGuizmo/ImGuizmo.cpp"
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
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"

    buildoptions { "/utf-8" }


	defines
	{
		"VZ_PLATFORM_WINDOWS", 
		"GLFW_INCLUDE_NONE",
		"VZ_BUILD_DLL",
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE"
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
		"Vesper/src/Vesper",
		"Vesper/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Vesper",
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


project "Vesper-Editor"
	location "Vesper-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"Vesper/vendor/ImGuizmo/ImGuizmo.h",
	}

	filter "files:Vesper/vendor/ImGuizmo/ImGuizmo.cpp"
	flags { "NoPCH" }
	filter {}

	includedirs
	{
		"Vesper/vendor/spdlog/include",
		"Vesper/src",
		"Vesper/src/Vesper",
		"Vesper/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
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
			"VZ_PLATFORM_WINDOWS",
			"VZ_EDITOR_USE_DEFAULT_SCENE"
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