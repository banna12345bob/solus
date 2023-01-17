#pragma once

#include "Solus/Renderer/OrthographicCamera.h"

namespace Solus {
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const float rotation, const glm::vec2 size, const glm::vec4 colour);
		static void DrawQuad(const glm::vec3& position, const float rotation, const glm::vec2 size, const glm::vec4 colour);
	};

}