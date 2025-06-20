-- vendor/assimp/premake5.lua

newoption {
    trigger     = "assimp_shared",
    description = "Build Assimp as a shared library (DLL/.so)"
}

project "assimp"
    location "."
    -- pick StaticLib or SharedLib based on the newoption
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    -- all public headers + code
    files 
    {
        "include/**.h",
        "include/**.hpp",
        "include/**.inl",
        "code/**.h",
        "code/**.cpp",
    }

    includedirs 
    {
        "include",
        "code",
    }

    defines 
    {
        -- disable optional importers we don’t need
        "ASSIMP_BUILD_NO_M3D_IMPORTER",
        "ASSIMP_BUILD_NO_M3D_EXPORTER",
        "ASSIMP_BUILD_NO_USD_IMPORTER",
        "ASSIMP_BUILD_NO_VRML_IMPORTER",
    }

    -- copy+rename config.h.in → config.h before any compilation
    -- filter { }  -- clear any existing filters
    -- prebuildcommands 
    -- {
        -- for config.h
        -- "{COPYFILE} %{prj.location}/include/assimp/config.h.in %{prj.location}/include/assimp/config.h",
        -- for revision.h
        -- "{COPYFILE} %{prj.location}/include/assimp/revision.h.in %{prj.location}/include/assimp/revision.h",
    -- }


    filter "system:windows"
        systemversion "latest"
        defines { "WIN32_LEAN_AND_MEAN" }
        links   { "Ws2_32", "Winmm" }  -- Cineware / socket deps

    filter "configurations:Debug"
        defines { "ASSIMP_DEBUG" }
        runtime  "Debug"
        symbols  "on"

    filter "configurations:Release"
        defines { "ASSIMP_RELEASE" }
        runtime  "Release"
        optimize "on"

    filter "configurations:Dist"
        defines { "ASSIMP_DIST" }
        runtime  "Release"
        optimize "On"
        symbols "off"
