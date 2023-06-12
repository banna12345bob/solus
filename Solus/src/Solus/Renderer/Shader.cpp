#include "supch.h"
#include "Solus/Renderer/Shader.h"

#include "Solus/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/SDL/SDLShader.h"

namespace Solus {
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    SU_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return CreateRef<OpenGLShader>(filepath);
		case RenderAPI::API::SDL:     return CreateRef<SDLShader>(filepath);
		}

		SU_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:    SU_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::API::OpenGL:  return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
			case RenderAPI::API::SDL:     return CreateRef<SDLShader>(name, vertexSrc, fragmentSrc);
		}

		SU_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	void ShaderLibary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		SU_CORE_ASSERT(!Exists(name), "Shader already exists.");
		m_Shaders[name] = shader;
	}

	void ShaderLibary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibary::Get(const std::string& name)
	{
		SU_CORE_ASSERT(Exists(name), "Shader not found.");
		return m_Shaders[name];
	}

}