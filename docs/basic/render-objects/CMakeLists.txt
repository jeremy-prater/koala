set(PROJ_NAME "render-objects")
project(${PROJ_NAME})

find_package(RapidJSON)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Magnum REQUIRED GL Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Add Debuglogger to Koala
include_directories("../../../src/engine"
                    "../../../src/debuglogger"
                    "../../../src/objects"
                    ${Boost_INCLUDE_DIRS}
                    ${RAPIDJSON_INCLUDE_DIRS}
)

add_executable(${PROJ_NAME} render-objects.cpp)
set(CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJ_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJ_NAME} PRIVATE
    KoalaEngine
    Magnum::Application
    Magnum::GL
    Magnum::Magnum)

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})
