#include "pch.h"
#include "log.h"

namespace LearnOpenGL
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        s_CoreLogger = spdlog::stdout_color_mt("LearnOpenGL");
        s_CoreLogger->set_level(spdlog::level::trace);
    }
}