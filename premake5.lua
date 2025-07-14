workspace "ColleGUI"
    configurations { "Debug", "Release" }
    architecture "x86_64"

function common_library_settings()
    language "C"
    targetdir ("bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "src/**.c",
        "src/**.h"
    }

    includedirs { "include" }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter {} -- reset filter
end

project "ColleGUI"
    kind "StaticLib"
    common_library_settings()

project "ColleGUI"
    kind "SharedLib"

    filter "system:windows"
        defines { "MYLIBRARY_EXPORTS" } -- for __declspec(dllexport)

    filter {} -- reset
    common_library_settings()

