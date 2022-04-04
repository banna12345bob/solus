#include "supch.h"
#include "Application.h"

#include "Events/AllEvents.h"
#include "Solus/Log.h"

namespace Solus {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		SU_TRACE(e);
		while (true);
	}

}