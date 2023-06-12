#include "supch.h"
#include "Platform/SDL/SDLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <SDL.h>

namespace Solus {

	SDLContext::SDLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		SU_CORE_ASSERT(windowHandle, "Windows handle is null.");
	}

	void SDLContext::Init()
	{
		SU_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SU_CORE_ASSERT(status, "Failed to initalise glad");

		SU_CORE_TRACE("SDL Debug Infomation:");
		SU_CORE_INFO("  Platform: {0}", SDL_GetPlatform());
		//SU_CORE_INFO("  Renderer: {0}", );
		SDL_version ver;
		SDL_GetVersion(&ver);
		SU_CORE_INFO("  Version: {0}.{1}.{2}", ver.major, ver.minor, ver.patch);
		SU_CORE_ASSERT(false, "SDL rendering is currently not supported. TODO: translate OpenGL calls to SDL.")

	#ifdef SU_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		SU_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Solus requires at least OpenGL version 4.5!");
	#endif
	}

	void SDLContext::SwapBuffers()
	{
		SU_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}


}