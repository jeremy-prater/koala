set(PROJ_NAME "render-objects")
project(${PROJ_NAME})

find_package(RapidJSON)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED Sdl2Application GL SceneGraph MeshTools Shaders Trade)
find_package(MagnumPlugins REQUIRED TinyGltfImporter)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Add Debuglogger to Koala
include_directories("${PROJECT_SOURCE_DIR}/../../../src/"
                    "${PROJECT_SOURCE_DIR}"
                    "${PROJECT_BINARY_DIR}/../../../magnum-plugins/src"
                    "${PROJECT_SOURCE_DIR}/../../../external/libzmq/include"
                    "${PROJECT_SOURCE_DIR}/../../../external/cppzmq"
                    ${Boost_INCLUDE_DIRS}
                    ${RAPIDJSON_INCLUDE_DIRS}
)

add_executable(${PROJ_NAME} render-objects.cpp)
set(CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJ_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJ_NAME} PRIVATE
    KoalaEngine
    Corrade::Main
    Magnum::Magnum
    Magnum::Application
    Magnum::GL
    Magnum::SceneGraph
    Magnum::MeshTools
    Magnum::Shaders
    Magnum::Trade
    Magnum::AnyImageImporter
    MagnumPlugins::TinyGltfImporter
    MagnumPlugins::DdsImporter
    MagnumPlugins::PngImporter)

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})
