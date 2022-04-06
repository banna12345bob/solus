#pragma once

#ifdef SU_PLATFORM_WINDOWS
	#ifdef SU_BUILD_DLL
		#define SOLUS_API __declspec(dllexport)
	#else
		#define SOLUS_API __declspec(dllimport)
	#endif
#elif SU_PLATFORM_LINUX
	#error Solus doesn't support Linux
#elif SU_PLATFORM_MAC
	#error Solus doesn't support Mac
#else
	#error What kind of operating system are you using? What ever it is it's not supported
#endif

#ifdef SU_DEBUG
	#define SU_ENABLE_ASSERTS
#endif

#ifdef SU_ENABLE_ASSERTS
#define SU_ASSERT(x, ...) { if(!(x)) { SU_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define SU_CORE_ASSERT(x, ...) { if(!(x)) { SU_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define SU_ASSERT(x, ...)
#define SU_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SU_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)