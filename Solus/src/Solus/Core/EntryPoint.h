#pragma once
#include "Solus/Core/Core.h"

#ifdef SU_PLATFORM_WINDOWS

extern Solus::Application* Solus::CreateApplication();

int main(int argc, char** argv)
{
	Solus::Log::Init();
	SU_CORE_INFO("Solus Initialised");
	SU_INFO("Build successful");

	SU_PROFILE_BEGIN_SESSION("Startup", "SolusProfile-Startup.json");
	auto app = Solus::CreateApplication();
	SU_PROFILE_END_SESSION();

	SU_PROFILE_BEGIN_SESSION("Runtime", "SolusProfile-Runtime.json");
	app->Run();
	SU_PROFILE_END_SESSION();

	SU_PROFILE_BEGIN_SESSION("Shutdown", "SolusProfile-Shutdown.json");
	delete app;
	SU_PROFILE_END_SESSION();
}

#endif