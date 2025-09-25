#pragma once

#include <glm/glm.hpp>

namespace Hazel::Math {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

	struct Rect
	{
		glm::vec2 position;
		glm::vec2 size;
	};
}
