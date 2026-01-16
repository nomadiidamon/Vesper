#pragma once

#include <string>

namespace Vesper {

	class FileDialogs
	{
	public:
		// These return empty string if cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

	class FileSystem
	{
	public:
		static void Initialize();
		static std::string GetCurrentWorkingDirectory();
		static std::string GetAbsolutePath(const std::string& relativePath);
		static std::string GetTravelingUpPath(const std::string& path);
		static bool IsInitialized() { return m_Initialized; }

		static bool m_Initialized;
		static std::string m_RootEngineDirectory;
		static std::string m_RootEditorDirectory;
		static std::string m_ResourcesDirectory;
		static std::string m_AssetsDirectory;
		static std::string m_ProjectsDirectory;
		static std::string m_CurrentProjectDirectory;
	};


};