#include "supch.h"
#include "Solus/Renderer/GraphicsContext.h"

#include "Solus/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/SDL/SDLContext.h"

namespace Solus {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    SU_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		case RenderAPI::API::SDL:     return CreateScope<SDLContext>(static_cast<GLFWwindow*>(window));
		}

		SU_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

