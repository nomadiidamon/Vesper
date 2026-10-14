#pragma once

#include <string>
#include "Vesper/Renderer/RendererAPI.h"
#include "Vesper/App/Window.h"

namespace Vesper {

	/// @struct ApplicationSettings
	/// @brief Holds configuration settings for the application, including window properties, directories, and rendering options.
	struct ApplicationSettings {
		/// @brief The name of the application.
		std::string ApplicationName = "Vesper Application";

		/// @brief The working directory of the application.
		std::string WorkingDirectory;

		/// @brief The project directory of the application.
		std::string ProjectDirectory;

		/// @brief The assets directory of the application.
		std::string AssetsDirectory;

		/// @brief The resources directory of the application.
		std::string ResourcesDirectory;

		/// @brief The renderer API to use for the application.
		RendererAPI::API RendererAPIType = RendererAPI::API::OpenGL;

		/// @brief The width of the application window.
		uint32_t Width = 1280;

		/// @brief The height of the application window.
		uint32_t Height = 720;

		/// @brief The window mode of the application (e.g., windowed, fullscreen, borderless).
		WindowMode Mode = WindowMode::Windowed;

		/// @brief Flag indicating whether ImGui is enabled for the application.
		bool EnableImGui = true;

		/// @brief Flag indicating whether vertical synchronization (VSync) is enabled for the application.
		bool EnableVSync = false;
	};
}