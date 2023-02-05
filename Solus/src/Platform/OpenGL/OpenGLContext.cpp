#include "supch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Solus {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		SU_CORE_ASSERT(windowHandle, "Windows handle is null.");
	}

	void OpenGLContext::Init()
	{
		SU_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SU_CORE_ASSERT(status, "Failed to initalise glad");

		SU_CORE_TRACE("OpenGL Debug Infomation:");
		SU_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		SU_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		SU_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

	#ifdef SU_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		SU_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Solus requires at least OpenGL version 4.5!");
	#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		SU_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}


}