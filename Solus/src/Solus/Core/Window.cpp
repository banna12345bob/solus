#include "supch.h"
#include "Solus/Core/Window.h"

#ifdef SU_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Solus
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef SU_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		SU_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}