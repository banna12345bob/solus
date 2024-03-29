#pragma once

#include "Solus/Renderer/RenderCommand.h"

#include "Solus/Renderer/OrthographicCamera.h"
#include "Solus/Renderer/Shader.h"

namespace Solus {
	
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void onWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

	private:
		struct ScreenData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<ScreenData> m_ScreenData;
	};

}