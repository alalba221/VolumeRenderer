project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	staticruntime "on"
   openmp "On"
    --staticruntime "off"
	files { "src/**.h", "src/**.inl","src/**.cpp" }

   includedirs
   {
      "%{wks.location}/Alalba_8190/src",
	   "%{wks.location}/Alalba_8190/vendor",
      
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.tinyexr}",
		"%{IncludeDir.startup}",
		"%{IncludeDir.perlin_noise}",
   }
   -- prebuildcommands [[for %%i in (Shaders\*.vert Shaders\*.frag Shaders\*.comp ) do (C:\VulkanSDK\1.3.236.0\Bin\glslc.exe %%i -o "%%~dpi\%%~ni.spv")]]
    links
    {
        "Alalba_8190"
    }

    defines
	{
      "NOMINMAX",
	}
   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "ALALBA_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "ALALBA_DEBUG" }
      runtime "Debug"
      symbols "On"
      
      links
		{

		}
        postbuildcommands 
		{

		}

   filter "configurations:Release"
      defines { "ALALBA_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"
      links
		{

		}
       postbuildcommands 
		{

		}


   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "ALALBA_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"
      links
		{

		}
       postbuildcommands 
		{

		}