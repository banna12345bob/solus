#pragma once

#if SU_PLATFORM_WINDOWS

extern Solus::Application* Solus::CreateApplication();

int main(int argc, char** argv)
{
	Solus::Log::Init();
	SU_CORE_INFO("Windows Solus Initialised");
	SU_INFO("Windows build successful");

	auto app = Solus::CreateApplication();
	app->Run();
	delete app;
}

#elif SU_PLATFORM_LINUX
extern Solus::Application* Solus::CreateApplication();

int main(int argc, char** argv)
{
    Solus::Log::Init();
	SU_CORE_INFO("Linux Solus Initialised");
    SU_INFO("Linux build successful");

    auto app = Solus::CreateApplication();
    app->Run();
    delete app;
}

#endif