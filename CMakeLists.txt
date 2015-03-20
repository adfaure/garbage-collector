cmake_minimum_required (VERSION 3.1)
project (garbage-collector)

#VERSION
set (garbage-collector_VERSION_MAJOR 0)
set (garbage-collector_VERSION_MINOR 1)

# DEBUG ONLY
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")

# FLAGS
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Weffc++ -pedantic")

# SOURCES
set(SOURCE_FILES src/main.cpp src/IGarbageCollector.cpp
    src/IGarbageCollector.hpp src/garbage_collector.hpp
    src/garbage_collector.cpp src/smart_ptr.cpp
    src/smart_ptr.hpp src/generique_pointer.hpp src/generique_pointer.cpp)

# EXECUTABLE
add_executable(Garbage_collector ${SOURCE_FILES})
