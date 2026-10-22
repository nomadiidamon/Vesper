include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Vesper"
	architecture "x64"

	startproject "Cavernae"

	configurations 
	{ 
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["entt"] = "%{wks.location}/Vesper/vendor/entt/include"
IncludeDir["Gateware"] = "%{wks.location}/Vesper/vendor/Gateware"
IncludeDir["Glad"] = "%{wks.location}/Vesper/vendor/Glad/include"
IncludeDir["GLFW"] = "%{wks.location}/Vesper/vendor/GLFW/include"
IncludeDir["glm"] = "%{wks.location}/Vesper/vendor/glm"
IncludeDir["googletest"] = "%{wks.location}/Vesper/vendor/googletest/googletest/include"
IncludeDir["googlemock"] = "%{wks.location}/Vesper/vendor/googletest/googlemock/include"
IncludeDir["ImGui"] = "%{wks.location}/Vesper/vendor/imgui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Vesper/vendor/ImGuizmo"
IncludeDir["stb_image"] = "%{wks.location}/Vesper/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Vesper/vendor/yaml-cpp/include"
IncludeDir["Resources"] = "%{wks.location}/Resources"

group "Dependencies"
	include "Vesper/vendor/GLFW"
	include "Vesper/vendor/Glad"
	include "Vesper/vendor/imgui"
	include "Vesper/vendor/yaml-cpp"
	include "Vesper/vendor/googletest"

group ""


group "Core"
	include "Vesper"
group ""


group "Tools"
	include "Cavernae"
group ""


group "Tests"
	include "Tests"
group ""


group "Misc"
	include "Sandbox"
group ""
