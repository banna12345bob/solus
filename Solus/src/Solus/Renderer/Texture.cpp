#include "supch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Solus {

	Ref<Texture2D> Texture2D::Create(const std::string & path)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:    SU_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
		}

		SU_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}