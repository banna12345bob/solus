#pragma once

#include "Solus/Core/Core.h"

#include "Solus/Events/AllEvents.h"
#include "Solus/Events/Event.h"
#include "Solus/Core/Window.h"
#include "Solus/Layers/LayerStack.h"

namespace Solus {

	class SOLUS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	//	To be defined in client
	Application* CreateApplication();
}

