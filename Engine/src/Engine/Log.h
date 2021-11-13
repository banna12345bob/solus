// SPDLog: https://github.com/gabime/spdlog

#pragma once

#include<memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Engine {

	class ENGINE_API Log
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

/* ------------------------- Core log macros ------------------------- */
#define EN_CORE_TRACE(...)   ::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EN_CORE_INFO(...)    ::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EN_CORE_WARN(...)    ::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EN_CORE_ERROR(...)   ::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EN_CORE_FATAL(...)   ::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)

/* ------------------------ Client log macros ------------------------ */
#define EN_TRACE(...)        ::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EN_INFO(...)         ::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define EN_WARN(...)         ::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EN_ERROR(...)        ::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
#define EN_FATAL(...)        ::Engine::Log::GetClientLogger()->critical(__VA_ARGS__)