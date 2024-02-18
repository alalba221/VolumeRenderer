project "Alalba_8190"
	kind "StaticLib"
	--kind "SharedLib"

	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	openmp "On"
	--staticruntime "off"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src/**.h",
		"src/**.inl",
		"src/**.cpp",
	}

	defines
	{
		--"TINYEXR_IMPLEMENTATION",
		"_CRT_SECURE_NO_WARNINGS",
	}

	includedirs
	{
		"src",
		"src/pch",

		"%{IncludeDir.spdlog}",
		"%{IncludeDir.tinyexr}",
		"%{IncludeDir.startup}",
		"%{IncludeDir.tinyobj}",
		
	}

	links
	{
		"startup",
	}

	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ALALBA_PLATFORM_WINDOWS",
			"NOMINMAX",
		}

	filter "configurations:Debug"
		defines "ALALBA_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{

		}
		postbuildcommands 
		{

		}


	filter "configurations:Release"
		defines "ALALBA_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{

		}
		postbuildcommands 
		{

		}


	filter "configurations:Dist"
		defines "ALALBA_DIST"
		runtime "Release"
		optimize "on"

		links
		{

		}
		postbuildcommands 
		{

		}