#include "supch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Solus {
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    SU_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return new OpenGLShader(filepath);
		}

		SU_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:    SU_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		SU_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}