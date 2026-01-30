#pragma once

#include "Vesper/Core/Base.h"
#include <glm/glm.hpp>


namespace Vesper {

	/// @class Input
	/// @brief Base input class for querying input states.
	class Input
	{
	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		/// @brief Checks if the specified key is currently pressed.
		///
		/// @param keycode The keycode of the key to check.
		/// @return True if the key is pressed, false otherwise.
		static bool IsKeyPressed(int keycode);

		/// @brief Checks if the specified mouse button is currently pressed.
		///
		/// @param button The mouse button to check.
		/// @return True if the mouse button is pressed, false otherwise.
		static bool IsMouseButtonPressed(int button);

		/// @brief Gets the current X position of the mouse cursor.
		static float GetMouseX();
		/// @brief Gets the current Y position of the mouse cursor.
		static float GetMouseY();
		/// @brief Gets the current position of the mouse cursor as a 2D vector.
		static glm::vec2 GetMousePosition();
	};
}