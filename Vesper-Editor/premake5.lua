project "Vesper-Editor"
	location "Vesper-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp",
	}

	filter "files:Vesper/vendor/ImGuizmo/ImGuizmo.cpp"
	flags { "NoPCH" }
	filter {}

	includedirs
	{
		"%{wks.location}/Vesper/vendor/spdlog/include",
		"%{wks.location}/Vesper/src",
		"%{wks.location}/Vesper/vendor",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Resources}"
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