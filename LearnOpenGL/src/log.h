#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace LearnOpenGL
{
	class Log {
	public:
		static void Init(); 

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private: 
		static std::shared_ptr<spdlog::logger> s_CoreLogger; 
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// define as macors to reduce 
// Core log macros
#define TY_CORE_TRACE(...)	:: LearnOpenGL::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TY_CORE_INFO(...)	:: LearnOpenGL::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TY_CORE_WARN(...)	:: LearnOpenGL::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TY_CORE_ERROR(...)	:: LearnOpenGL::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TY_CORE_FATAL(...)	:: LearnOpenGL::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// client log macros
#define TY_TRACE(...)		:: LearnOpenGL::Log::GetlientLLogger()->trace(__VA_ARGS__)
#define TY_INFO(...)		:: LearnOpenGL::Log::GetClientLogger()->info(__VA_ARGS__)
#define TY_WARN(...)		:: LearnOpenGL::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TY_ERROR(...)		:: LearnOpenGL::Log::GetlientLLogger()->error(__VA_ARGS__)
#define TY_CORE_FATAL(...)	:: LearnOpenGL::Log::GetlientLLogger()->fatal(__VA_ARGS__)
