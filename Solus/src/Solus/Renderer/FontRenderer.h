#pragma once

#include "Solus/Core/Core.h"

#include <glm/glm.hpp>

namespace Solus {
	class FontRenderer
	{
	public:
		static Ref<FontRenderer> Create(const std::string fontPath);

		static void RenderText(const std::string& text, const glm::vec2& position, const float& rotation, const glm::vec2& scale, const glm::vec4& colour);
	private:
	};

}