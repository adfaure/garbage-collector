-- A solution contains projects, and defines the available configurations
solution "Mif31Garbage_colletor"
    configurations { "Debug", "Release"}
    platforms { "native", "x64", "universal" }

   -- A project defines one build target
    project "Garbage_collector"
        kind "ConsoleApp"
        language "C++"
        files { "src/*" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        flags { "Symbols" }
        configuration { "linux", "gmake" }
            buildoptions {"-Wall", "-std=c++11","-Weffc++", "-ansi", "-pedantic"}
