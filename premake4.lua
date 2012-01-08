solution "Terra_Game_Engine"
	configurations { "Release", "Debug" }
	
	project "Terra"
		kind "WindowedApp"
		language "C++"
		files { "**.hpp", "**.cpp" }
		location "build"
		links { "sfml-graphics", "sfml-window", "sfml-system" }
		platforms { "x32", "x64" }
		includedirs { "./include" }
		defines { "TIXML_USE_STL" }
		
		configuration "Debug"
			flags { "Symbols" }
		
		configuration "Release"
			flags { "Optimize" }
		
		configuration "gmake"
			buildoptions { "--std=c++0x" }
		
		configuration { "windows", "x32" }
			targetdir "./build/win32"
			libdirs "./lib/win32"
			os.mkdir "./build/win32/resources"
			os.copyfile( "./lib/win32/**.dll", "./build/win32" )
			os.copyfile( "./resources/**", "./build/win32/resources" )
		
		configuration { "linux", "x32" }
			targetdir "./build/lin32"
			libdirs "./lib/lin32"
			os.mkdir "./build/lin32/libs"
			os.mkdir "./build/lin32/resources"
			os.copyfile( "./scripts/linux/**", "./build/lin32" )
			os.copyfile( "./lib/lin32/**", "./build/lin32/libs" )
			os.copyfile( "./resources/**", "./build/lin32/resources" )
		
		configuration { "windows", "x64" }
			targetdir "./build/win64"
			libdirs "./lib/win64"
			os.mkdir "./build/win64/resources"
			os.copyfile( "./lib/win64/**.dll", "./build/win64" )
			os.copyfile( "./resources/**", "./build/win64/resources" )
		
		configuration { "linux", "x64" }
			targetdir "./build/lin64"
			libdirs "./lib/lin64"
			os.mkdir "./build/lin64/libs"
			os.mkdir "./build/lin64/resources"
			os.copyfile( "./scripts/linux/**", "./build/lin64" )
			os.copyfile( "./lib/lin64/**", "./build/lin64/libs" )
			os.copyfile( "./resources/**", "./build/lin64/resources" )
