#include "supch.h"
#include "Solus/Renderer/Buffer.h"

#include "Solus/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Solus {
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:		SU_CORE_ASSERT(false, "RenderAPI::None is not supported"); return nullptr;
		case RenderAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(size);
		}

		SU_CORE_ASSERT(false, "Unknown graphics api");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:		SU_CORE_ASSERT(false, "RenderAPI::None is not supported"); return nullptr;
			case RenderAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		SU_CORE_ASSERT(false, "Unknown graphics api");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:		SU_CORE_ASSERT(false, "RenderAPI::None is not supported"); return nullptr;
			case RenderAPI::API::OpenGL:	return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		SU_CORE_ASSERT(false, "Unknown graphics api");
		return nullptr;
	}

}