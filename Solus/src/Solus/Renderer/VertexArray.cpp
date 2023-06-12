#include "supch.h"
#include "Solus/Renderer/VertexArray.h"

#include "Solus/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/SDL/SDLVertexArray.h"

namespace Solus {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    SU_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		case RenderAPI::API::SDL:     return std::make_shared<SDLVertexArray>();
		}

		SU_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}