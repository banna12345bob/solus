#include "supch.h"
#include "Solus/Core/Application.h"

#include "Solus/Core/Log.h"

#include "Solus/Renderer/Renderer.h"

#include "Input.h"

#include <GLFW/glfw3.h>

namespace Solus {

	Application* Application::s_Instance = nullptr;

	Application::Application(WindowProps props)
	{
		SU_PROFILE_FUNCTION();
		SU_CORE_ASSERT(!s_Instance, "Application already running");
		s_Instance = this;
		m_Window = Window::Create(props);
		m_Window->SetEventCallback(SU_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		SU_PROFILE_FUNCTION();
		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		SU_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	
	void Application::PushOverlay(Layer* overlay)
	{
		SU_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		SU_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SU_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SU_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		SU_PROFILE_FUNCTION();
		while (m_Running)
		{
			SU_PROFILE_SCOPE("Run loop");
			float time = (float)glfwGetTime(); // Platform::GetTime() (eventually)
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimised)
			{
				SU_PROFILE_SCOPE("LayerStack OnUpdate");
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				SU_PROFILE_SCOPE("LayerStack OnImGuiRender");
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		SU_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimised = true;
			return false;
		}

		m_Minimised = false;
		Renderer::onWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}