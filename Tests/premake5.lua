project "Tests"
	location "Tests"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	debugdir ("%{wks.location}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"%{wks.location}/Vesper/vendor/spdlog/include",
		"%{wks.location}/Vesper/src",
		"%{wks.location}/Vesper/vendor",
		"%{IncludeDir.Gateware}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Resources}",
		"%{IncludeDir.googletest}",
		"%{IncludeDir.googlemock}"
	}

	-- Vesper headers rely on the precompiled header for Ref, std types, etc.
	forceincludes { "vzpch.h" }

	links
	{
		"Vesper",
		"GoogleTest"
	}

	filter "system:windows"
		systemversion "latest"

		buildoptions { "/utf-8" }

		defines
		{
			"VZ_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
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
