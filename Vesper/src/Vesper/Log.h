#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"


namespace Vesper {

	class VESPER_API Log
	{
	public:

		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};


}

// Core log macros
#define VZ_CORE_TRACE(...)    ::Vesper::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VZ_CORE_INFO(...)     ::Vesper::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VZ_CORE_WARN(...)     ::Vesper::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VZ_CORE_ERROR(...)    ::Vesper::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VZ_CORE_FATAL(...)    ::Vesper::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define VZ_TRACE(...)         ::Vesper::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VZ_INFO(...)          ::Vesper::Log::GetClientLogger()->info(__VA_ARGS__)
#define VZ_WARN(...)          ::Vesper::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VZ_ERROR(...)         ::Vesper::Log::GetClientLogger()->error(__VA_ARGS__)
#define VZ_FATAL(...)         ::Vesper::Log::GetClientLogger()->critical(__VA_ARGS__)
