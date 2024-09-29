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