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

    local function addCMakeBuild(cfgFilter)
        local cmakeFlags = {
            "-DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=ON",
            "-DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=ON",
            "-DASSIMP_BUILD_ZLIB=ON",
            "-DASSIMP_INSTALL=OFF",
            "-DBUILD_SHARED_LIBS=OFF",
            "-DUSE_STATIC_CRT=ON",
            "-DCMAKE_DEBUG_POSTFIX=",
            "-DLIBRARY_SUFFIX=",
            "-DCMAKE_BUILD_TYPE=" .. cfgFilter,
        }
        local flagsString = table.concat(cmakeFlags, " ")

        -- 1) Configure step: Generate a VS solution
        buildmessage(" Generating VS solution and building binary (static lib)...")
        buildcommands {
            "cmake -S " .. assimpdir ..
            " -B " .. assimpbuilddir ..
            " -G \"Visual Studio 16 2019\" -A x64" ..
            " " .. flagsString
        }

        -- 2) Build step
        buildcommands {
            "cmake --build " .. assimpbuilddir .. " --config " .. cfgFilter
        }

        -- 3) Tell Premake what file to expect as output
        buildoutputs { 
            path.join(assimpbuilddir, "assimp.lib"),
            path.join(assimpbuilddir, (cfgFilter=="Debug" and "zlibstaticd.lib" or "zlibstatic.lib")),
        }

        -- 4) copy libs to bin/ directory
        -- copy Assimp static lib
        local assimplib = path.join(assimpbuilddir, "lib", cfgFilter, "assimp.lib")
        postbuildmessage (('{COPY} "%s" "%s"'):format(assimplib, bindir))
        postbuildcommands {
            ('{COPY} "%s" "%s"'):format(assimplib, bindir),
        }
        
        -- copy ZLib static lib
        local zliblib   = path.join(assimpbuilddir, "contrib", "zlib", cfgFilter)
        local zlibname  = (cfgFilter=="Debug" and "zlibstaticd.lib" or "zlibstatic.lib")
        postbuildmessage (('{COPY} "%s"->"%s"'):format(zliblib, bindir))
        postbuildcommands{
            ('{COPY} "%s" "%s"'):format(path.join(zliblib, zlibname), bindir),
        }
    end

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        addCMakeBuild("Debug")
    filter "configurations:Release"
        runtime "Release"
        optimize "speed"
        addCMakeBuild("Release")
    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
        symbols "off"
        addCMakeBuild("Release")