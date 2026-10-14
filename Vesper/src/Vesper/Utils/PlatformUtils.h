#pragma once
/// @file PlatformUtils.h
/// @author Damon S. Green II
/// @brief Declares platform-specific utilities for file dialogs and file system operations.

#include <string>

namespace Vesper {
	
	/// @class FileDialogs
	/// @brief Cross-platform file dialog utilities.
	class FileDialogs
	{
	public:
		/// @brief Opens a file dialog to select a file to open.
		///
		/// @param filter The file type filter for the dialog.
		/// @return The selected file path or an empty string if cancelled.
		static std::string OpenFile(const char* filter);

		/// @brief Opens a file dialog to select a location to save a file.
		///
		/// @param filter The file type filter for the dialog.
		/// @return The selected file path or an empty string if cancelled.
		static std::string SaveFile(const char* filter);
	};

};