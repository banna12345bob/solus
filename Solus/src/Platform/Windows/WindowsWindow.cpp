#include "supch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Solus/Events/AllEvents.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "Solus/Renderer/Renderer.h"

#include <stb_image.h>

namespace Solus {

	static uint32_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		SU_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		SU_PROFILE_FUNCTION();
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		SU_PROFILE_FUNCTION();
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		SU_PROFILE_FUNCTION();
		//Sets window's title, width and height to default
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		SU_CORE_INFO("Creating window {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);

		if (s_GLFWWindowCount == 0)
		{
			SU_CORE_INFO("Initialising GLFW");
			int success = glfwInit();
			SU_CORE_ASSERT(success, "Could not intialise GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			SU_PROFILE_SCOPE("glfwCreateWindow");
			#if defined(SU_DEBUG)
				if (Renderer::GetAPI() == RenderAPI::API::OpenGL)
					glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			#endif
			if (props.fullscreen && !(props.nativeResulution))
			{
				m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), glfwGetPrimaryMonitor(), nullptr);
			}
			else if (props.fullscreen && props.nativeResulution)
			{
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
				m_Window = glfwCreateWindow((int)mode->width, (int)mode->height, m_Data.Title.c_str(), glfwGetPrimaryMonitor(), nullptr);
			}
			else
			{
				m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
			}

			s_GLFWWindowCount++;
		}

		m_Context = OpenGLContext::Create(m_Window);
		m_Context->Init();

		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		if (props.pathToIcon != "")
		{
			GLFWimage images[1];
			images[0].pixels = stbi_load(props.pathToIcon, &images[0].width, &images[0].height, 0, 4); //rgba channels 
			if (images[0].pixels)
			{
				glfwSetWindowIcon(m_Window, 1, images);
			}
			else
			{
				SU_CORE_ERROR("Failed to load image: {0}", props.pathToIcon);
			}
			stbi_image_free(images[0].pixels);
		}
		
		//Event callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		SU_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);
		if (s_GLFWWindowCount == 0)
		{
			SU_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		SU_PROFILE_FUNCTION();
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		SU_PROFILE_FUNCTION();
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}