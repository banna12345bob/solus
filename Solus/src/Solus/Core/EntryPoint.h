#pragma once

#ifdef SU_PLATFORM_WINDOWS

extern Solus::Application* Solus::CreateApplication();

int main(int argc, char** argv)
{
	Solus::Log::Init();
	SU_CORE_INFO("Solus Initialised");
	SU_INFO("Build successful");

	auto app = Solus::CreateApplication();
	app->Run();
	delete app;
}

#endif