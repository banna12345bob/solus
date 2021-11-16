#include <Solus.h>

class Sandbox : public Solus::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

Solus::Application* Solus::CreateApplication()
{
	return new Sandbox();
}