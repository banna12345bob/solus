#pragma once

#include <glm/glm.hpp>

#include "Solus/Renderer/VertexArray.h"

namespace Solus {

	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1, SDL = 2
		};
	public:
		virtual void Init() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) = 0;

		inline static API GetAPI() { return s_API; }
		static Scope<RenderAPI> Create();
	private:
		static API s_API;
	};

}