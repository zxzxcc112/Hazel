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

#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
	#define HZ_CLIENT_ASSERT(x, ...) { if(!(x)) { HZ_CLIENT_ERROR("Assertion Failed:{0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed:{0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_CLIENT_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(N) (1 << N)

#define HZ_BIND_EVENT_FN(Fn) std::bind(&Fn, this, std::placeholders::_1)