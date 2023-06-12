#include "supch.h"
#include "Solus/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"
#include "Platform/SDL/SDLRenderAPI.h"

namespace Solus {

	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

	Scope<RenderAPI> RenderAPI::Create()
	{
		switch (s_API)
		{
		case RenderAPI::API::None:    SU_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return CreateScope<OpenGLRenderAPI>();
		case RenderAPI::API::SDL:	  return CreateScope<SDLRenderAPI>();
		}

		SU_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}