cmake_minimum_required(VERSION 3.4)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add SDL2 library
message("Finding SDL2")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add Corrade as a subproject
message("Finding Corrade")
add_subdirectory(corrade EXCLUDE_FROM_ALL)

# Add Magnum as a subproject, enable Sdl2Application
message("Finding Magnum")
set(WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
add_subdirectory(magnum EXCLUDE_FROM_ALL)

add_subdirectory(src)
add_subdirectory(docs)
add_subdirectory(tests)
