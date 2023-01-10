#pragma once

#include "Core.h"

#include "Window.h"
#include "Solus/Layers/LayerStack.h"
#include "Solus/Events/Event.h"
#include "Solus/Events/ApplicationEvent.h"

#include "Solus/Core/Timestep.h"

#include "Solus/ImGui/ImGuiLayer.h"


namespace Solus {

	class SOLUS_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

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