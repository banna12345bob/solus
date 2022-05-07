#pragma once

#include "Solus/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Solus {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}