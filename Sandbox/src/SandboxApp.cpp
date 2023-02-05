#include <Solus.h>
#include <Solus/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Solus::Application
{
public:
	Sandbox(Solus::WindowProps props)
		: Solus::Application(props)
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{

	}
};

Solus::Application* Solus::CreateApplication()
{
	Solus::WindowProps props(Solus::WindowProps("Sandbox", 1280, 720, "assets/textures/oak_log.png"));
	return new Sandbox(props);
}