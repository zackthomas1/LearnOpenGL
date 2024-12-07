--premake5.lua 
workspace "ToyEngine" 
    architecture "x64"
    startproject "ToyEngine"

    configurations 
    { 
        "Debug", 
        "Release", 
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "ToyEngine/vendor/GLFW/include"
IncludeDir["glad"] = "ToyEngine/vendor/glad/include"
IncludeDir["stb"] = "ToyEngine/vendor/stb"
IncludeDir["glm"] = "ToyEngine/vendor/glm"
IncludeDir["spdlog"] = "ToyEngine/vendor/spdlog/include"

group "Dependencies"
    include "ToyEngine/vendor/GLFW"
    include "ToyEngine/vendor/glad"
group ""

project "ToyEngine"
    location "ToyEngine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "ToyEngine/src/pch.cpp"

files
{
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
}

--[[
    defines{

    }
]]

includedirs
{
    "%{prj.name}/src",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.glad}",
    "%{IncludeDir.stb}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.spdlog}",
}

links
{
    "GLFW",
    "glad",
    "opengl32.lib",
}

filter "system:windows"
    systemversion "latest"
    buildoptions { "/utf-8" }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "PLATFORM_WINDOWS"
    }

    filter "configurations:Debug"
        defines {"TY_DEBUG", "TY_ENABLE_ASSERTS"}
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines {"TY_RELEASE"}
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines {"TY_DIST"}
        runtime "Release"
        optimize "On"