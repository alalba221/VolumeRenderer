project "startup"
	kind "StaticLib"
	
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
	    "include/**.cpp",
      	"include/**.h",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
			-- "src/win32_init.c",
			-- "src/win32_joystick.c",
			-- "src/win32_monitor.c",
			-- "src/win32_time.c",
			-- "src/win32_thread.c",
			-- "src/win32_window.c",
			-- "src/wgl_context.c",
			-- "src/egl_context.c",
			-- "src/osmesa_context.c"
		}

		defines 
		{ 
			-- "_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"