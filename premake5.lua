workspace "ColleGUI"
    configurations { "Debug", "Release" }
    location "build"

project "ColleGUI"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    files { "src/**.cpp", "include/**.h" }
    includedirs { "include" }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG" }