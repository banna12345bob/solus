#include "supch.h"
#include "Solus/Core/Input.h"

#ifdef SU_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif

namespace Solus {

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef SU_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		SU_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}