#pragma once


#ifdef VZ_PLATFORM_WINDOWS
	#ifdef VZ_BUILD_DLL
		#define VESPER_API __declspec(dllexport)
	#else
		#define VESPER_API __declspec(dllimport)
	#endif
#else
	#error Vesper only supports Windows!
#endif

#define BIT(x) (1 << x)

