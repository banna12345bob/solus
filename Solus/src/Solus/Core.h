#pragma once

#ifdef SU_PLATFORM_WINDOWS
	#ifdef SU_BUILD_DLL
		#define SOLUS_API __declspec(dllexport)
	#else
		#define SOLUS_API __declspec(dllimport)
	#endif
#else
	#error This is a Windows only engine
#endif