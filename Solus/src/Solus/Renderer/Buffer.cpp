#include "supch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Solus {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:		SU_CORE_ASSERT(false, "RenderAPI::None is not supported"); return nullptr;
			case RenderAPI::API::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
		}

		SU_CORE_ASSERT(false, "Unknown graphics api");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:		SU_CORE_ASSERT(false, "RenderAPI::None is not supported"); return nullptr;
			case RenderAPI::API::OpenGL:	return new OpenGLIndexBuffer(indices, size);
		}

		SU_CORE_ASSERT(false, "Unknown graphics api");
		return nullptr;
	}

}