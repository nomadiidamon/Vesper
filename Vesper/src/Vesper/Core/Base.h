#pragma once

#include <memory>

// Platform detection
#ifdef _WIN32
	/*Windows x64/x86*/
	#ifdef _WIN64
		/*Windows x64  */
	#ifndef VZ_PLATFORM_WINDOWS
		#define VZ_PLATFORM_WINDOWS
	#endif	
#else
		/*Windows x86*/
		#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/*Apple platforms */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS Simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define VZ_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define VZ_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
#endif
#elif defined(__ANDROID__)
	#define VZ_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define VZ_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/*Unknown compiler/platform*/
	#error "Unknown platform!"
#endif // End of platform detection


// DLL support
#ifdef VZ_PLATFORM_WINDOWS
	#if VZ_DYNAMIC_LINK
		#ifdef VZ_BUILD_DLL
			#define VESPER_API __declspec(dllexport)
		#else
			#define VESPER_API __declspec(dllimport)
		#endif
	#else
		#define VESPER_API
	#endif
#else
#error Vesper only supports Windows!
#endif // End of DLL support



#ifdef VZ_DEBUG
	#define VZ_ENABLE_ASSERTS
#endif	

#ifdef VZ_ENABLE_ASSERTS
	#define VZ_ASSERT(x, ...) { if(!(x)) { VZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define VZ_CORE_ASSERT(x, ...) { if(!(x)) { VZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define VZ_ASSERT(x, ...)
	#define VZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define VZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


namespace Vesper 
{
	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}