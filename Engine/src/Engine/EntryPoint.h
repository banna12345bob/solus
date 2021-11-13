#pragma once

#ifdef EN_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();
	EN_CORE_WARN("Logging Initialised");
	int a = 5;
	EN_INFO("Build successful Var={0}", a);

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif
