--premake5.lua 
workspace "LearnOpenGL" 
    architecture "x64"
    startproject "LearnOpenGL"

    configurations 
    { 
        "Debug", 
        "Release", 
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
vendordir = "%{wks.location}/LearnOpenGL/vendor"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"]      = "LearnOpenGL/vendor/GLFW/include"
IncludeDir["glad"]      = "LearnOpenGL/vendor/glad/include"
IncludeDir["stb"]       = "LearnOpenGL/vendor/stb"
IncludeDir["glm"]       = "LearnOpenGL/vendor/glm"
IncludeDir["spdlog"]    = "LearnOpenGL/vendor/spdlog/include"
IncludeDir["assimp"]    = "LearnOpenGL/vendor/assimp/include"
IncludeDir["assimp_build"]    = "LearnOpenGL/vendor/assimp/build/" .. outputdir .. "/include"

LibDir = {}
LibDir["assimp"] = "LearnOpenGL/vendor/assimp/bin/" .. outputdir .. "/assimp"

group "Dependencies"
    include "LearnOpenGL/vendor/GLFW"
    include "LearnOpenGL/vendor/glad"
    include "LearnOpenGL/vendor/premake/assimp.lua"
group ""

project "LearnOpenGL"
    location "LearnOpenGL"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "LearnOpenGL/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    defines{

    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.assimp_build}"
    }

    libdirs
    {
        "%{LibDir.assimp}",
    }

    links
    {
        "GLFW",
        "glad",
        "opengl32.lib",
        "assimp",
    }

    postbuildcommands
    {
        ("{COPYDIR} %{wks.location}/assets %{cfg.targetdir}/../assets")
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
        links { "zlibstaticd" } -- link the debug‐suffix zlib

    filter "configurations:Release"
        defines {"TY_RELEASE"}
        runtime "Release"
        optimize "On"
        links { "zlibstatic" }  -- link the release zlib

    filter "configurations:Dist"
        defines {"TY_DIST"}
        runtime "Release"
        optimize "On"
        links { "zlibstatic" }  -- link the release zlib