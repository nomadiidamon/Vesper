#include "vzpch.h"
#include "ResourceManager.h"
#include <filesystem>
#include "Application.h"


namespace Vesper {

	ResourceManager::ResourceManager(ApplicationSettings settings)
	{
		InitFileSystem(settings);
	}
	ResourceManager::~ResourceManager()
	{
	}

	std::string ResourceManager::ResolvePath(const std::string& Path, PathResolveMode Mode) const
	{
		if (Path.empty())
		{
			VZ_CORE_WARN("ResolvePath called with empty path.");
			return std::string();
		}

		std::string resolvedPath;
		switch (Mode)
		{
		case PathResolveMode::WorkingDirectory:
			resolvedPath = ResolvePath_WorkingDirectory(Path);
			break;
		case PathResolveMode::ProjectDirectory:
			resolvedPath = ResolvePath_ProjectDirectory(Path);
			break;
		case PathResolveMode::AssetsDirectory:
			resolvedPath = ResolvePath_AssetsDirectory(Path);
			break;
		default:
			VZ_CORE_WARN("ResolvePath called with unknown PathResolveMode.");
			resolvedPath = std::string();
			break;
		}
		return resolvedPath;
	}

	std::string ResourceManager::ResolvePath_WorkingDirectory(const std::string& Path) const
	{
		std::string resultPath;

		if (WorkingDirectory.empty())
		{
			VZ_CORE_WARN("WorkingDirectory is empty, cannot resolve path.");
			return std::string();
		}

		if (Path.empty())
		{
			VZ_CORE_WARN("ResolvePath_WorkingDirectory called with empty path.");
			return std::string();
		}

		if (IsAbsolutePath(Path))
		{
			VZ_CORE_WARN("ResolvePath_WorkingDirectory called with an absolute path: {0}", Path);
			return NormalizePath(Path);
		}

		resultPath = JoinPaths(WorkingDirectory, Path);
		return resultPath;
	}

	std::string ResourceManager::ResolvePath_ProjectDirectory(const std::string& Path) const
	{
		std::string resultPath;
		if (ProjectDirectory.empty())
		{
			VZ_CORE_WARN("ProjectDirectory is empty, cannot resolve path.");
			return std::string();
		}
		if (Path.empty())
		{
			VZ_CORE_WARN("ResolvePath_ProjectDirectory called with empty path.");
			return std::string();
		}
		if (IsAbsolutePath(Path))
		{
			VZ_CORE_WARN("ResolvePath_ProjectDirectory called with an absolute path: {0}", Path);
			return NormalizePath(Path);
		}
		resultPath = JoinPaths(ProjectDirectory, Path);
		return resultPath;
	}

	std::string ResourceManager::ResolvePath_AssetsDirectory(const std::string& Path) const
	{
		std::string resultPath;
		if (AssetsDirectory.empty())
		{
			VZ_CORE_WARN("AssetsDirectory is empty, cannot resolve path.");
			return std::string();
		}
		if (Path.empty())
		{
			VZ_CORE_WARN("ResolvePath_AssetsDirectory called with empty path.");
			return std::string();
		}
		if (IsAbsolutePath(Path))
		{
			VZ_CORE_WARN("ResolvePath_AssetsDirectory called with an absolute path: {0}", Path);
			return NormalizePath(Path);
		}
		resultPath = JoinPaths(AssetsDirectory, Path);
		return resultPath;
	}

	std::string ResourceManager::ResolvePath_ResourcesDirectory(const std::string& Path) const
	{
		std::string resultPath;
		if (ResourcesDirectory.empty())
		{
			VZ_CORE_WARN("ResourcesDirectory is empty, cannot resolve path.");
			return std::string();
		}
		if (Path.empty())
		{
			VZ_CORE_WARN("ResolvePath_ResourcesDirectory called with empty path.");
			return std::string();
		}
		if (IsAbsolutePath(Path))
		{
			VZ_CORE_WARN("ResolvePath_ResourcesDirectory called with an absolute path: {0}", Path);
			return NormalizePath(Path);
		}
		resultPath = JoinPaths(ResourcesDirectory, Path);
		return resultPath;
	}

	VesperResult ResourceManager::InitFileSystem(ApplicationSettings& Settings)
	{
		VesperResult result;

		/// Create the file system
		if (GW::GReturn::SUCCESS != FileSystem.Create())
		{
			result = VesperResult(VZ_RETURN::FAILURE, "Failed to create file system.");
			VZ_CORE_ERROR("Failed to create file system.");
			return result;
		}

		/// Setup the working directory
		char currentWorkingDirectory[255];
		if (Settings.WorkingDirectory.empty())
		{
			if (GW::GReturn::SUCCESS != FileSystem.GetCurrentWorkingDirectory(currentWorkingDirectory, sizeof(currentWorkingDirectory)))
			{
				result = VesperResult(VZ_RETURN::FAILURE, "Failed to get current working directory.");
				VZ_CORE_ERROR("Failed to get current working directory.");
				return result;
			}
			else
			{
				Settings.WorkingDirectory = NormalizePath(currentWorkingDirectory);
				VZ_CORE_INFO("Current working directory: {0}", Settings.WorkingDirectory);
			}
		}
		else
		{
			if (GW::GReturn::SUCCESS != FileSystem.SetCurrentWorkingDirectory(NormalizePath(Settings.WorkingDirectory).c_str()))
			{
				result = VesperResult(VZ_RETURN::FAILURE, "Failed to set current working directory.");
				VZ_CORE_ASSERT(false, "Failed to set current working directory.");
				return result;
			}
			else
			{
				VZ_CORE_INFO("Current working directory: {0}", Settings.WorkingDirectory);
			}
		}
		/// Ensure working directory exists
		std::filesystem::path workDir = Settings.WorkingDirectory;
		if (!std::filesystem::exists(workDir))
		{
			result = VesperResult(VZ_RETURN::FAILURE, "Working directory does not exist.");
			VZ_CORE_ERROR("Working directory does not exist: {0}", Settings.WorkingDirectory);
			return result;
		}
		else
		{
			VZ_CORE_INFO("Working directory exists: {0}", Settings.WorkingDirectory);
		}
		

		/// Setup the project directory
		if (!Settings.ProjectDirectory.empty() && !IsAbsolutePath(Settings.ProjectDirectory))
		{
			Settings.ProjectDirectory = JoinPaths(Settings.WorkingDirectory, Settings.ProjectDirectory);
			VZ_CORE_INFO("Resolved relative Project directory to: {0}", Settings.ProjectDirectory);
		}
		if (Settings.ProjectDirectory.empty())
		{
			Settings.ProjectDirectory = JoinPaths(Settings.WorkingDirectory, "Project");
			VZ_CORE_INFO("Project Directory is: {0}", Settings.ProjectDirectory);
		}
		else
		{
			VZ_CORE_INFO("Project Directory is: {0}", Settings.ProjectDirectory);
		}
		/// Ensure project directory exists
		std::filesystem::path projDir = Settings.ProjectDirectory;
		if (!std::filesystem::exists(projDir))
		{
			if (std::filesystem::create_directories(projDir))
			{
				VZ_CORE_INFO("Project directory created: {0}", Settings.ProjectDirectory);
			}
			else
			{
				result = VesperResult(VZ_RETURN::FAILURE, "Failed to create project directory.");
				VZ_CORE_ERROR("Failed to create project directory: {0}", Settings.ProjectDirectory);
				return result;
			}
		}
		else
		{
			VZ_CORE_INFO("Project directory already exists: {0}", Settings.ProjectDirectory);
		}


		/// Setup the assets directory
		if (!Settings.AssetsDirectory.empty() && !IsAbsolutePath(Settings.AssetsDirectory))
		{
			Settings.AssetsDirectory = JoinPaths(Settings.WorkingDirectory, Settings.AssetsDirectory);
			VZ_CORE_INFO("Resolved relative Assets directory to: {0}", Settings.AssetsDirectory);
		}
		if (Settings.AssetsDirectory.empty())
		{
			Settings.AssetsDirectory = JoinPaths(Settings.ProjectDirectory, "Assets");
			VZ_CORE_INFO("Assets Directory was empty. It defaulted to: {0}", Settings.AssetsDirectory);
		}
		else
		{
			VZ_CORE_INFO("Assets Directory is: {0}", Settings.AssetsDirectory);
		}
		/// Ensure assets directory exists
		std::filesystem::path assetsDir = Settings.AssetsDirectory;
		if (!std::filesystem::exists(assetsDir))
		{
			if (std::filesystem::create_directories(assetsDir))
			{
				VZ_CORE_INFO("Assets directory created: {0}", Settings.AssetsDirectory);
			}
			else
			{
				result = VesperResult(VZ_RETURN::FAILURE, "Failed to create assets directory.");
				VZ_CORE_ERROR("Failed to create assets directory: {0}", Settings.AssetsDirectory);
				return result;
			}
		}
		else
		{
			VZ_CORE_INFO("Assets directory already exists: {0}", Settings.AssetsDirectory);
		}


		/// Setup the resources directory
		std::filesystem::path resourcesDir = Settings.ResourcesDirectory;
		if (resourcesDir.empty())
		{
			resourcesDir = JoinPaths(WorkingDirectory, "Vesper");
			resourcesDir = JoinPaths(resourcesDir.string(), "src");
			resourcesDir = JoinPaths(resourcesDir.string(), "Resources");

			if (!std::filesystem::exists(resourcesDir))
			{
				result = VesperResult(VZ_RETURN::FAILURE, "Failed to locate the Engine resources directory.");
				VZ_CORE_ERROR("Failed to locate the Engine resources directory: {0}", resourcesDir.string());
				return result;
			}
			else
			{
				VZ_CORE_INFO("Resources directory located successfully: {0}", resourcesDir.string());
			}
		}	
		/// Ensure resources directory exists
		if (!std::filesystem::exists(resourcesDir))
		{
			if (std::filesystem::create_directories(resourcesDir))
			{
				VZ_CORE_INFO("Resources directory created: {0}", Settings.ResourcesDirectory);
			}
			else
			{
				result = VesperResult(VZ_RETURN::FAILURE, "Failed to create resources directory.");
				VZ_CORE_ERROR("Failed to create resources directory: {0}", Settings.ResourcesDirectory);
				return result;
			}
		}
		else
		{
			VZ_CORE_INFO("Resources directory already exists: {0}", Settings.ResourcesDirectory);
		}


		/// Update the ResourceManager's directories with the validated settings
		WorkingDirectory = Settings.WorkingDirectory;
		ProjectDirectory = Settings.ProjectDirectory;
		AssetsDirectory = Settings.AssetsDirectory;
		ResourcesDirectory = Settings.ResourcesDirectory;

		return result;
	}

}