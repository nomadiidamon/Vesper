#pragma once

#include <filesystem>
#include "Vesper/Core/VesperResult.h"
#include "Vesper/App/ApplicationSettings.h"
#include "Gateware.h"


/// @file ResourceManager.h
/// @author Damon S. Green II
/// @brief Manages the loading and retrieval of resources in the application.

namespace Vesper {


	/// @enum PathResolveMode
	/// @brief Specifies the mode for resolving resource paths in the application.
	enum class PathResolveMode {
		WorkingDirectory,
		ProjectDirectory,
		AssetsDirectory,
		ResourcesDirectory
	};


	/// @class ResourceManager
	/// @brief Manages the loading and retrieval of resources in the application.
	class ResourceManager
	{
	public:
		ResourceManager(ApplicationSettings settings);
		~ResourceManager();

		/// @brief Resolves the full path to a resource based on a relative path and the application's settings.
		///
		/// @param Path The path to the resource.
		/// @param Mode The mode to use for resolving the path. Defaults to PathResolveMode::AssetsDirectory.
		std::string ResolvePath(const std::string& Path, PathResolveMode Mode = PathResolveMode::AssetsDirectory) const;

		/// @brief Resolves the full path to a resource based on the application's working directory.
		///
		/// @param Path The path to the resource.
		std::string ResolvePath_WorkingDirectory(const std::string& Path) const;

		/// @brief Resolves the full path to a resource based on the application's project directory.
		///
		/// @param Path The path to the resource.
		std::string ResolvePath_ProjectDirectory(const std::string& Path) const;

		/// @brief Resolves the full path to a resource based on the application's assets directory.
		///
		/// @param Path The path to the resource.
		std::string ResolvePath_AssetsDirectory(const std::string& Path) const;

		/// @brief Resolves the full path to a resource based on the application's resources directory.
		///
		/// @param Path The path to the resource.
		std::string ResolvePath_ResourcesDirectory(const std::string& Path) const;

		/// @brief Normalizes path separators to forward slashes for cross-platform consistency
		static std::string NormalizePath(const std::string& path)
		{
			std::string normalized = path;
			for (char& c : normalized)
			{
				if (c == '\\')
					c = '/';
			}
			return normalized;
		}

		/// @brief Joins two path segments with proper separator handling
		static std::string JoinPaths(const std::string& base, const std::string& relative)
		{
			if (base.empty())
				return relative;
			if (relative.empty())
				return base;

			std::string result = base;

			// Ensure base doesn't end with a separator
			if (result.back() == '/' || result.back() == '\\')
				result.pop_back();

			result += '/';

			// Append relative path, removing leading separators
			size_t start = 0;
			while (start < relative.size() && (relative[start] == '/' || relative[start] == '\\'))
				start++;

			result += relative.substr(start);
			return result;
		}

		/// @brief Determines whether the given path is an absolute path (Unix-style, Windows-style, or drive-letter).
		static bool IsAbsolutePath(const std::string& path)
		{
			if (path.empty())
				return false;

			if (path[0] == '/' || path[0] == '\\')
				return true;

			if (path.size() > 1 && path[1] == ':')
				return true;

			return false;
		}

	public:
		/// @brief Retrieves the working directory of the application as a string.
		std::string GetWorkingDirectory() const { return WorkingDirectory; }
		/// @brief Retrieves the working directory of the application as a std::filesystem::path.
		std::filesystem::path GetWorkingDirectoryPath() const { return std::filesystem::path(WorkingDirectory); }

		/// @brief Retrieves the project directory of the application as a string.
		std::string GetProjectDirectory() const { return ProjectDirectory; }
		/// @brief Retrieves the project directory of the application as a std::filesystem::path.
		std::filesystem::path GetProjectDirectoryPath() const { return std::filesystem::path(ProjectDirectory); }

		/// @brief Retrieves the assets directory of the application as a string.
		std::string GetAssetsDirectory() const { return AssetsDirectory; }
		/// @brief Retrieves the assets directory of the application as a std::filesystem::path.
		std::filesystem::path GetAssetsDirectoryPath() const { return std::filesystem::path(AssetsDirectory); }

		/// @brief Retrieves the resources directory of the application as a string.
		std::string GetResourcesDirectory() const { return ResourcesDirectory; }
		/// @brief Retrieves the resources directory of the application as a std::filesystem::path.
		std::filesystem::path GetResourcesDirectoryPath() const { return std::filesystem::path(ResourcesDirectory); }

	private:
		///	@brief Initializes the file system for the application with the given settings.
		///
		/// @param Settings The settings to configure the file system.
		VesperResult InitFileSystem(ApplicationSettings& Settings);

	private:
		/// @brief The working directory of the application.
		std::string WorkingDirectory;

		/// @brief The project directory of the application.
		std::string ProjectDirectory;

		/// @brief The assets directory of the application.
		std::string AssetsDirectory;

		/// @brief The resources directory of the application.
		std::string ResourcesDirectory;

		/// @brief The file system interface for the application.
		GW::SYSTEM::GFile FileSystem;

		friend class Application;

	};


}