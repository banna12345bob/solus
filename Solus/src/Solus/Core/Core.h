#pragma once

#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		#define SU_PLATFORM_WINDOWS
	#else
		#error "Unsupported Windows platform"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS Sim not supported"
	#elif TARGET_OS_IPHONE == 1
		#define SU_PLATFORM_IOS
		#error "Solus doesn't support IOS"
	#elif TARGET_OS_MAC == 1
		#define SU_PLATFORM_MACOS
		#error "Solus doesn't support MacOS"
	#else
		#error "Unknown Apple platform!"
	#endif

#elif defined(__ANDROID__)
	#define SU_PLATFORM_ANDROID
	#error "Solus doesn't support Android"
#elif defined(__linux__)
	#define SU_PLATFORM_LINUX
	#error "Solus doesn't support Linux"
#else
	#error "What kind of platform are you using? What ever it is it's not supported"
#endif

#ifdef SU_DEBUG
	#if defined(SU_PLATFORM_WINDOWS)
		#define SU_DEBUGBREAK() __debugbreak()
	#elif defined(SU_PLATFORM_LINUX)
		#include <signal.h>
		#define SU_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
		#define SU_ENABLE_ASSERTS
	#else
	#define SU_DEBUGBREAK()
#endif

#ifdef SU_ENABLE_ASSERTS
#define SU_ASSERT(x, ...) { if(!(x)) { SU_ERROR("Assertion Failed: {0}", __VA_ARGS__); SU_DEBUGBREAK(); } }
#define SU_CORE_ASSERT(x, ...) { if(!(x)) { SU_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); SU_DEBUGBREAK(); } }
#else
#define SU_ASSERT(x, ...)
#define SU_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SU_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Solus {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}