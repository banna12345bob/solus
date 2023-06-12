#include "supch.h"
#include "Platform/SDL/SDLRenderAPI.h"

#include <SDL.h>
#include <glad/glad.h>



namespace Solus {

	void SDLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         SU_CORE_FATAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       SU_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          SU_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: SU_CORE_TRACE(message); return;
		}

		SU_CORE_ASSERT(false, "Unknown severity level!");
	}

	void SDLRenderAPI::Init()
	{
		#ifdef SU_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(SDLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif
		SU_PROFILE_FUNCTION();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void SDLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
		//SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, nullptr, nullptr);
	}

	void SDLRenderAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void SDLRenderAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void SDLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}