project "miniz"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		--"tinyexr.h",
		--"tinyexr.c",
		"miniz.h",
		"miniz.c",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		files
		{

		}

		defines
		{

		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{

		}

		defines 
		{ 

		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
-- tinyexr

project "tinyexr"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"miniz.h",
		"miniz.c",
		"tinyexr.h",
		"tinyexr.cpp",
	}

	includedirs
	{
		
		"%{IncludeDir.tinyexr}",
	}
	defines
	{
		"TINYEXR_USE_MINIZ=1",
		"_CRT_SECURE_NO_WARNINGS"
	}
	links
	{
		"miniz",
	}
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		files
		{

		}

		defines
		{

		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{

		}

		defines 
		{ 

		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"