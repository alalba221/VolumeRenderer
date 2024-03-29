-- premake5.lua
include "Dependencies.lua"

workspace "Alalba_8190"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    targetdir "build"
    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
group "Dependencies"
include "Alalba_8190/vendor/startupCode"
group ""

include "Alalba_8190"
include "Sandbox"
include "Noise"