#pragma once

#include "Solus/Renderer/OrthographicCamera.h"
#include "Solus/Renderer/Texture.h"
#include "Solus/Renderer/SubTexture2D.h"

namespace Solus {
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const glm::vec4& colour);
		static void DrawQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const glm::vec4& colour);
		static void DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColour = glm::vec4(1.0f), const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColour = glm::vec4(1.0f), const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec2& position, const float& rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColour = glm::vec4(1.0f), const float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const float& rotation, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColour = glm::vec4(1.0f), const float tilingFactor = 1.0f);
		
		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t squareCount = 0;

			uint32_t GetTotalVertexCount() { return squareCount * 4; }
			uint32_t GetTotalIndexCount() { return squareCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:
		static void FlushAndReset();
	
	};

}