#include <Solus.h>

#include "ImGui/imgui.h"

class ExampleLayer : public Solus::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		
	}

	virtual void OnImGuiRender() override {
		/*ImGui::Begin("test");
		ImGui::Text("This is a test");
		ImGui::End();*/
	}

	void OnEvent(Solus::Event& event) override
	{
		//SU_TRACE("{0}", event);
	}
};

class Sandbox : public Solus::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

Solus::Application* Solus::CreateApplication()
{
	return new Sandbox();
}