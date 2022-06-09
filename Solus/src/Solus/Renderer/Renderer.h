#pragma once

#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"

namespace Solus {
	
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

	private:
		struct ScreenData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static ScreenData* m_ScreenData;
	};

}