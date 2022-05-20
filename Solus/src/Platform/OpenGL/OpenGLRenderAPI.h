#pragma once

#include "Solus/Renderer/RenderAPI.h"

namespace Solus {

	class OpenGLRenderAPI : public RenderAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};


}