#pragma once

/// ASSERT MACROS
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