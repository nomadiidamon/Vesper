#pragma once

#include <glm/glm.hpp>

namespace Vesper {

	namespace Color {

		static glm::vec4 White() { return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); }
		static glm::vec4 Black() { return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); }
		static glm::vec4 Gray() { return glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); }

		static glm::vec4 Red() { return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); }
		static glm::vec4 Orange() { return glm::vec4(1.0f, 0.5f, 0.0f, 1.0f); }
		static glm::vec4 Yellow() { return glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); }
		static glm::vec4 Green() { return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); }
		static glm::vec4 Blue() { return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); }
		static glm::vec4 Indigo() { return glm::vec4(0.29f, 0.0f, 0.51f, 1.0f); }
		static glm::vec4 Purple() { return glm::vec4(0.5f, 0.0f, 0.5f, 1.0f); }

		static glm::vec4 Cyan() { return glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); }
		static glm::vec4 Magenta() { return glm::vec4(1.0f, 0.0f, 1.0f, 1.0f); }
		static glm::vec4 Pink() { return glm::vec4(1.0f, 0.75f, 0.8f, 1.0f); }
		static glm::vec4 Brown() { return glm::vec4(0.6f, 0.4f, 0.2f, 1.0f); }
		static glm::vec4 Transparent() { return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); }

		static glm::vec4 StripAlpha(const glm::vec4& color) {	return glm::vec4(color.x, color.y, color.z, 1.0f);	}
		static glm::vec4 SetAlpha(const glm::vec4& color, float alpha = 0.0f) {	return glm::vec4(color.x, color.y, color.z, alpha);}
	}

}