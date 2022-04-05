#include <Solus.h>

class ExampleLayer : public Solus::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		SU_INFO("ExampleLayer : Update");
	}

	void OnEvent(Solus::Event& event) override
	{
		SU_TRACE("{0}", event);
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