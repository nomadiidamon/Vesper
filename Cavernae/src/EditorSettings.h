#pragma once

#include <Vesper.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <map>
#include <string>

namespace Vesper {

	/// @brief Loads Cavernae settings from an ini file into settings; missing keys keep their current values.
	/// @return true if the file was found and parsed.
	inline bool LoadEditorSettings(const std::string& path, ApplicationSettings& settings)
	{
		std::ifstream file(path);
		if (!file.is_open())
			return false;

		auto trim = [](std::string s) {
			auto notSpace = [](unsigned char c) { return !std::isspace(c); };
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
			s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
			return s;
		};
		auto lower = [](std::string s) {
			std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)std::tolower(c); });
			return s;
		};

		std::map<std::string, std::string> values; // "section.key" (lowercase) -> value
		std::string section, line;
		while (std::getline(file, line))
		{
			line = trim(line);
			if (line.empty() || line[0] == ';' || line[0] == '#')
				continue;
			if (line.front() == '[' && line.back() == ']')
			{
				section = lower(trim(line.substr(1, line.size() - 2)));
				continue;
			}
			size_t eq = line.find('=');
			if (eq == std::string::npos)
				continue;
			values[section + "." + lower(trim(line.substr(0, eq)))] = trim(line.substr(eq + 1));
		}

		auto get = [&](const char* key, std::string& out) {
			auto it = values.find(key);
			if (it == values.end()) return false;
			out = it->second;
			return true;
		};
		auto getBool = [&](const char* key, bool& out) {
			std::string v;
			if (!get(key, v)) return;
			v = lower(v);
			out = (v == "true" || v == "1" || v == "yes" || v == "on");
		};
		auto getUInt = [&](const char* key, uint32_t& out) {
			std::string v;
			if (!get(key, v)) return;
			try { out = (uint32_t)std::stoul(v); } catch (...) {}
		};

		get("application.name", settings.ApplicationName);
		get("application.projectdirectory", settings.ProjectDirectory);
		get("application.assetsdirectory", settings.AssetsDirectory);
		get("application.resourcesdirectory", settings.ResourcesDirectory);

		getUInt("window.width", settings.Width);
		getUInt("window.height", settings.Height);
		getBool("window.vsync", settings.EnableVSync);

		std::string v;
		if (get("window.mode", v))
		{
			v = lower(v);
			if (v == "windowed") settings.Mode = WindowMode::Windowed;
			else if (v == "fullscreen") settings.Mode = WindowMode::Fullscreen;
			else if (v == "borderless") settings.Mode = WindowMode::Borderless;
		}
		if (get("renderer.api", v))
		{
			v = lower(v);
			if (v == "opengl") settings.RendererAPIType = RendererAPI::API::OpenGL;
			else if (v == "none") settings.RendererAPIType = RendererAPI::API::None;
		}
		getBool("renderer.enableimgui", settings.EnableImGui);

		return true;
	}
}
