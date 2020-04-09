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
find_package(cppzmq)
find_package(ZeroMQ)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED Sdl2Application GL SceneGraph MeshTools Shaders Trade)
find_package(MagnumPlugins REQUIRED TinyGltfImporter)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Setup include directories
include_directories("${PROJECT_SOURCE_DIR}"
                    "${PROJECT_BINARY_DIR}/../magnum-plugins/src"
                    "${PROJECT_SOURCE_DIR}/../external/libzmq/include"
                    "${PROJECT_SOURCE_DIR}/../external/cppzmq"
                    ${Boost_INCLUDE_DIRS}
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${PROJECT_BINARY_DIR}
                    ${MAGNUMPLUGINS_INCLUDE_DIR}
                    )

# Add Debug logger
set(DEBUGLOGGER_SRCS "debuglogger/debuglogger.cpp")

# Add Koala engine
set(KOALA_ENGINE_SRCS "engine/engine.cpp")

# Add Koala objects
set(KOALA_OBJECT_SRCS "objects/base-object.cpp"
                      "objects/camera.cpp"
                      "objects/renderable.cpp"
                      "objects/scene-renderable-groups.cpp"
                      "objects/scene.cpp")

# Add Koala assets
set(KOALA_ASSET_SRCS "assets/project-configuration.cpp"
                     "assets/base-asset.cpp"
                     "assets/base-group.cpp"
                     "assets/gltf-asset.cpp"
                     "assets/glsl-asset.cpp"
                     )

# Add Koala Cloud
set(KOALA_CLOUD_SRCS "cloud/cloud.cpp"
                     "cloud/primary-endpoint.cpp"
                     "cloud/debugger.cpp")

set(SOURCE_FILES ${DEBUGLOGGER_SRCS}
                 ${KOALA_ENGINE_SRCS}
                 ${KOALA_OBJECT_SRCS}
                 ${KOALA_ASSET_SRCS}
                 ${KOALA_CLOUD_SRCS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_library(${PROJ_NAME} STATIC ${SOURCE_FILES})
target_link_libraries(${PROJ_NAME} PRIVATE
    libzmq
    Corrade::Main
    Magnum::Magnum
    Magnum::Application
    Magnum::GL
    Magnum::SceneGraph
    Magnum::MeshTools
    Magnum::Shaders
    Magnum::Trade
    MagnumPlugins::TinyGltfImporter)