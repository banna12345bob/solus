#include "Application.h"

#include "Solus/Events/ApplicationEvent.h"
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
		if (e.IsInCategory(EventCategoryApplication))
		{
			SU_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			SU_TRACE(e);
		}
		while (true);
	}

}