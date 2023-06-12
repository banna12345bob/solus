#include "supch.h"
#include "Platform/SDL/SDLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Solus {

	static GLenum ShaderTypeFromString(const std::string type)
	{
		if (type == "vertex") 
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		SU_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	SDLShader::SDLShader(const std::string& filepath)
	{
		SU_PROFILE_FUNCTION();
		std::string source = ReadFile(filepath);
		auto shaderSources = preProcess(source);
		Compile(shaderSources);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind(".");
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	SDLShader::SDLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		SU_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	SDLShader::~SDLShader()
	{
		SU_PROFILE_FUNCTION();
		glDeleteProgram(m_RendererID);
	}

	std::string SDLShader::ReadFile(const std::string& filepath)
	{
		SU_PROFILE_FUNCTION();
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				SU_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			SU_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> SDLShader::preProcess(std::string source)
	{
		SU_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			SU_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			SU_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			SU_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}

	void SDLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		SU_PROFILE_FUNCTION();
		GLuint program = glCreateProgram();
		SU_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourcecStr = (const GLchar*)source.c_str();
			glShaderSource(shader, 1, &sourcecStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				SU_CORE_ERROR("{0}", infoLog.data());
				SU_CORE_ASSERT(false, "Failed to compile shader!");
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			SU_CORE_ERROR("{0}", infoLog.data());
			SU_CORE_ASSERT(false, "Failed to link shaders!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		
		m_RendererID = program;
	}

	void SDLShader::Bind() const
	{
		SU_PROFILE_FUNCTION();
		glUseProgram(m_RendererID);
	}

	void SDLShader::Unbind() const
	{
		SU_PROFILE_FUNCTION();
		glUseProgram(0);
	}

	void SDLShader::SetInt(const std::string& name, int value)
	{
		SU_PROFILE_FUNCTION();
		UploadUniformInt(name, value);
	}

	void SDLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		SU_PROFILE_FUNCTION();
		UploadUniformIntArray(name, values, count);
	}

	void SDLShader::SetFloat(const std::string& name, float value)
	{
		SU_PROFILE_FUNCTION();
		UploadUniformFloat(name, value);
	}

	void SDLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		SU_PROFILE_FUNCTION();
		UploadUniformFloat3(name, value);
	}

	void SDLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		SU_PROFILE_FUNCTION();
		UploadUniformFloat4(name, value);
	}

	void SDLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		SU_PROFILE_FUNCTION();
		UploadUniformMat4(name, value);
	}

	void SDLShader::UploadUniformInt(const std::string& name, int values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, values);
	}

	void SDLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}
	
	void SDLShader::UploadUniformFloat(const std::string& name, float values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, values);
	}
	
	void SDLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}
	
	void SDLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void SDLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void SDLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	
	void SDLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}