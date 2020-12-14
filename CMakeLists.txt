cmake_minimum_required(VERSION 3.4)
project("Koala")

#Wonky work around for boost until it's fixed
#set(Boost_NO_BOOST_CMAKE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#Magnum config
set(WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
        "${CMAKE_SOURCE_DIR}/corrade/modules"
        "${CMAKE_SOURCE_DIR}/magnum/modules"
        "${CMAKE_SOURCE_DIR}/magnum-plugins/modules"
        "${PROJECT_SOURCE_DIR}/modules/")

# Magnum Plug-ins config
set(BUILD_PLUGINS_STATIC ON CACHE BOOL "" FORCE)
set(WITH_ANYIMAGEIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_DDSIMPORTER ON CACHE BOOL "" FORCE)
# set(WITH_JPEGIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_PNGIMPORTER ON CACHE BOOL "" FORCE)
# set(WITH_ANYSCENEIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_TINYGLTFIMPORTER ON CACHE BOOL "" FORCE)

# Add SDL2 library
message("Finding SDL2")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add Corrade as a subproject
message("Finding Corrade")
add_subdirectory(corrade)

# Add Magnum as a subproject, enable Sdl2Application
message("Finding Magnum")
add_subdirectory(magnum)

# Add external deps
message("Adding externals")
SET(BUILD_STATIC ON CACHE BOOL "" FORCE)
SET(WITH_PERF_TOOL OFF CACHE BOOL "" FORCE)
SET(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)
SET(ZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)
SET(WITH_TLS OFF CACHE BOOL "" FORCE)
SET(ZeroMQ_DIR "${CMAKE_CURRENT_BINARY_DIR}/external/libzmq" CACHE STRING "" FORCE)
SET(cppzmq_DIR "${CMAKE_CURRENT_BINARY_DIR}/external/cppzmq" CACHE STRING "" FORCE)
SET(PNG_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib/libpng16d.lib" CACHE STRING "" FORCE)
SET(PNG_PNG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/libpng" CACHE STRING "" FORCE)

include_directories("${PROJECT_SOURCE_DIR}"
                    "${PROJECT_BINARY_DIR}/../magnum-plugins/src"
                    "${PROJECT_SOURCE_DIR}/external/libzmq/include"
                    "${PROJECT_SOURCE_DIR}/external/cppzmq"
                    "${CMAKE_CURRENT_BINARY_DIR}/external/libpng")

add_subdirectory(external)

# Add Magnum plugins as a subproject
message("Finding Magnum Plugins")
add_subdirectory(magnum-plugins)

# Add Koala engine
message("Adding Koala Source")
add_subdirectory(src)

message("Adding Koala Docs")
add_subdirectory(docs)

message("Adding Koala Tests")
add_subdirectory(tests)
