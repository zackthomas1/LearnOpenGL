--premake5.lua 
workspace "FluidSimulator" 
    architecture "x64"
    startproject "FluidSimulator"

    configurations 
    { 
        "Debug", 
        "Release", 
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "FluidSimulator/vendor/GLFW/include"
IncludeDir["glad"] = "FluidSimulator/vendor/glad/include"

group "Dependencies"
    include "FluidSimulator/vendor/GLFW"
    include "FluidSimulator/vendor/glad"
group ""

project "FluidSimulator"
    location "FluidSimulator"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

--pchheader 
--pchsource

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
}

links
{
    "GLFW",
    "glad",
    "opengl32.lib",
}

filter "system:windows"
    systemversion "latest"

    defines
    {
        "GLFW_INCLUDE_NONE"
    }

    filter "configurations:Debug"
        defines {"DEBUG"}
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        runtime "Release"
        optimize "On"