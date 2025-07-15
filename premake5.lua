workspace "ColleGUI"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    startproject "test"  -- デフォルト起動プロジェクト（任意）

function common_library_settings()
    language "C"
    targetdir ("bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

    files {
        "src/**.c",
        "src/collegui/gui.c",
        "include/**.h"
    }

    includedirs { "include" }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter "system:linux"
        links { "X11" }

    filter {} -- reset
end

project "ColleGUI"
    kind "StaticLib"
    common_library_settings()

    filter "system:windows"
        defines { "MYLIBRARY_EXPORTS" }

    filter {} -- reset

project "test"
    kind "ConsoleApp"
    language "C"
    targetdir ("bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("bin-int/%{cfg.buildcfg}/%{prj.name}")

    files { "test/**.c", "test/**.h" }

    includedirs { "include" }

    links { "ColleGUI" }
    dependson { "ColleGUI" }

    libdirs { "bin/%{cfg.buildcfg}/ColleGUI" } -- StaticLib or SharedLib output

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

    filter "system:linux"
        links { "X11" }

    filter {} -- reset

