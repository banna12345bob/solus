// SPDLog: https://github.com/gabime/spdlog

#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Solus {

	class SOLUS_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr < spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr < spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr < spdlog::logger> s_CoreLogger;
		static std::shared_ptr < spdlog::logger> s_ClientLogger;
	};

}

#ifdef SU_RELASE
/* ------------------------- Core log macros ------------------------- */
#define SU_CORE_TRACE(...)
#define SU_CORE_INFO(...)
#define SU_CORE_WARN(...)
#define SU_CORE_ERROR(...)
#define SU_CORE_FATAL(...)

/* ------------------------ Client log macros ------------------------ */
#define SU_TRACE(...)
#define SU_INFO(...)
#define SU_WARN(...)
#define SU_ERROR(...)
#define SU_FATAL(...)
#else
/* ------------------------- Core log macros ------------------------- */
#define SU_CORE_TRACE(...)   ::Solus::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SU_CORE_INFO(...)    ::Solus::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SU_CORE_WARN(...)    ::Solus::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SU_CORE_ERROR(...)   ::Solus::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SU_CORE_FATAL(...)   ::Solus::Log::GetCoreLogger()->critical(__VA_ARGS__)

/* ------------------------ Client log macros ------------------------ */
#define SU_TRACE(...)        ::Solus::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SU_INFO(...)         ::Solus::Log::GetClientLogger()->info(__VA_ARGS__)
#define SU_WARN(...)         ::Solus::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SU_ERROR(...)        ::Solus::Log::GetClientLogger()->error(__VA_ARGS__)
#define SU_FATAL(...)        ::Solus::Log::GetClientLogger()->critical(__VA_ARGS__)
#endif