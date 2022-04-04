#pragma once

#ifdef SU_PLATFORM_WINDOWS
	#ifdef SU_BUILD_DLL
		#define SOLUS_API __declspec(dllexport)
	#else
		#define SOLUS_API __declspec(dllimport)
	#endif
#ifdef SU_PLATFORM_LINUX
	#error Solus doesn't support Linux
#endif
#else
	#error This is a Windows only engine
#endif

#define BIT(x) (1 << x)