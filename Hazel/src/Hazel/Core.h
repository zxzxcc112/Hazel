#pragma once

/*Define marcos for different platforms*/

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif // HZ_BUILD_DLL
#elif defined(HZ_PLATFORM_MAC) || defined(HZ_PLATFORM_LINUX)
	//for macOS and Linux
#else
#error "Hazel not support!"
#endif

#define BIT(N) (1 << N)