#pragma once

#ifdef EN_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();
	EN_CORE_INFO("Engine Initialised");
	EN_TRACE("Build successful");

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif