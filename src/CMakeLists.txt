set(PROJ_NAME "KoalaEngine")
project(${PROJ_NAME})

# The version number.
execute_process(
  COMMAND git rev-parse HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

set (${PROJ_NAME}_VERSION_MAJOR 1)
set (${PROJ_NAME}_VERSION_MINOR 0)
set (${PROJ_NAME}_BUILD_SHA ${GIT_COMMIT_HASH})

# configure a header file to pass some of the CMake settings
configure_file (
  "${PROJECT_SOURCE_DIR}/koala-version.hpp.in"
  "${PROJECT_BINARY_DIR}/koala-version.hpp"
  )


find_package(RapidJSON)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Magnum REQUIRED GL Sdl2Application)
find_package(MagnumPlugins REQUIRED TinyGltfImporter)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Setup include directories
include_directories("debuglogger"
                    "engine"
                    "objects"
                    "${PROJECT_BINARY_DIR}/../magnum-plugins/src"
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS}
                    ${PROJECT_BINARY_DIR}
                    ${MAGNUMPLUGINS_INCLUDE_DIR}
                    )

# Add Debug logger
set(DEBUGLOGGER_SRCS "debuglogger/debuglogger.cpp")

# Add Koala engine
set(KOALAENGINE_SRCS "engine/koala-engine.cpp") 

# Add Koala objects
set(KOALAOBJECT_SRCS "objects/project-configuration.cpp"
                     "objects/koala-object.cpp"
                     "objects/gltf-object.cpp"
                     "objects/glsl-object.cpp")

# Add Parsers
set(KOALAPARSERS_SRCS "") 

set(SOURCE_FILES ${DEBUGLOGGER_SRCS} ${KOALAENGINE_SRCS} ${KOALAOBJECT_SRCS} ${KOALAPARSERS_SRCS})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_library(${PROJ_NAME} STATIC ${SOURCE_FILES})
target_link_libraries(${PROJ_NAME} PRIVATE
    Magnum::Application
    MagnumPlugins::TinyGltfImporter
    Magnum::GL
    Magnum::Magnum)