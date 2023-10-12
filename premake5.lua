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

--[[
group "Dependencies"
    include
group ""
]]

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

--[[
    defines{

    }
]]


includedirs
{
    "%{prj.name}/src",
}


--[[
    links{

    }
]]

filter "system:windows"
    systemversion "latest"

    filter "configurations:Debug"
        defines {"DEBUG"}
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        runtime "Release"
        optimize "On"