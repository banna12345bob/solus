#pragma once

#include "Core.h"

#include "Window.h"
#include "Solus/Layers/LayerStack.h"
#include "Solus/Events/Event.h"
#include "Solus/Events/ApplicationEvent.h"

#include "Solus/ImGui/ImGuiLayer.h"

#include "Solus/Renderer/Shader.h"
#include "Solus/Renderer/Buffer.h"
#include "Solus/Renderer/VertexArray.h"

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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}