#include "supch.h"
#include "OpenGLContext.h"

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
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SU_CORE_ASSERT(status, "Failed to initalise glad");

		SU_CORE_TRACE("OpenGL Debug Infomation:");
		SU_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		SU_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		SU_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}


}