#include "supch.h"
#include "Renderer.h"

namespace Solus {

	Renderer::ScreenData* Renderer::m_ScreenData = new Renderer::ScreenData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_ScreenData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_ScreenData->ViewProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}