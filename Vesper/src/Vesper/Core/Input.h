#pragma once

#include "Vesper/Core/Base.h"
#include <glm/glm.hpp>


namespace Vesper {

	class VESPER_API Input
	{
	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static glm::vec2 GetMousePosition();
	};
}