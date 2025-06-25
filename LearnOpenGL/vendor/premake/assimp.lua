local assimpdir         = (vendordir .. "/assimp")
local assimpbuilddir    = (assimpdir .. "/build/" .. outputdir)
local bindir            = (assimpdir .. "/bin/" .. outputdir .. "/%{prj.name}")
local binintdir         = (assimpdir .. "/bin-int/" .. outputdir .. "/%{prj.name}")

project "assimp"
    kind "StaticLib"
    language  "C++"
    location  (assimpdir)    -- where CMake will generate build files

    targetdir (bindir)
    objdir (binintdir)

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    filter "configurations:Release"
        runtime "Release"
        optimize "speed"
    filter{}
    
    local cmakeFlags = {
        "-DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=ON",
        "-DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=ON",
        "-DASSIMP_BUILD_ZLIB=ON",
        "-DASSIMP_INSTALL=OFF",
        "-DBUILD_SHARED_LIBS=OFF",
        "-DUSE_STATIC_CRT=ON",
        "-DCMAKE_DEBUG_POSTFIX=",
        "-DLIBRARY_SUFFIX=",
        "-DCMAKE_BUILD_TYPE=%{cfg.buildcfg}",
    }
    local flagsString = table.concat(cmakeFlags, " ")

    -- 1) Configure step: Generate a VS solution
    buildmessage(" Generating VS solution and building binary (static lib)...")
    local assimplib = path.join(assimpbuilddir, "lib", "%{cfg.buildcfg}", "assimp.lib")
    local zliblib   = path.join(assimpbuilddir, "contrib", "zlib", "%{cfg.buildcfg}", "zlibstaticd.lib")

    buildcommands 
    {
        "cmake -S " .. assimpdir ..
        " -B " .. assimpbuilddir ..
        " -G \"Visual Studio 16 2019\" -A x64" ..
        " " .. flagsString
    }

    -- 2) Build step
    buildcommands 
    {
        "cmake --build " .. assimpbuilddir .. " --config %{cfg.buildcfg}"
    }

    -- 3) Tell Premake what file to expect as output
    buildoutputs { "%{cfg.buildcfg}/assimp.lib" }

    -- 4) copy libs to bin/ directory
    local assimplib = path.join(assimpbuilddir, "lib", "%{cfg.buildcfg}", "assimp.lib")
    local zliblib   = path.join(assimpbuilddir, "contrib", "zlib", "%{cfg.buildcfg}", "zlibstaticd.lib")

    postbuildcommands
    {
        -- copy the Assimp static lib
        ('{COPY} "%s" "%s"'):format(assimplib, bindir),
        -- copy the ZLib static lib
        ('{COPY} "%s" "%s"'):format(zliblib,   bindir),
    }
    postbuildmessage (
        ('{COPY} "%s" "%s"'):format(assimplib, bindir)
        .. "\n" .. 
        ('{COPY} "%s" "%s"'):format(zliblib,   bindir),
    )
