#pragma once

#include "Solus/Core/Core.h"

#include "Solus/Core/Window.h"
#include "Solus/Core/LayerStack.h"
#include "Solus/Events/Event.h"
#include "Solus/Events/ApplicationEvent.h"

#include "Solus/Core/Timestep.h"

#include "Solus/ImGui/ImGuiLayer.h"


namespace Solus {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimised = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}