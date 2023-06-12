#include "supch.h"
#include "Solus/Renderer/Texture.h"

#include "Solus/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/SDL/SDLTexture.h"

namespace Solus {

	Ref<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:    SU_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height);
			case RenderAPI::API::SDL:     return CreateRef<SDLTexture2D>(width, height);
		}

		SU_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string & path)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:    SU_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
			case RenderAPI::API::SDL:     return CreateRef<SDLTexture2D>(path);
		}

		SU_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}