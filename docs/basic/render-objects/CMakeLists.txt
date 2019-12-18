set(PROJ_NAME "render-objects")
project(${PROJ_NAME})

find_package(RapidJSON)
find_package(Magnum REQUIRED GL Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Add Debuglogger to Koala
include_directories("../../../src/engine"
                    "../../../src/debuglogger"
                    "../../../src/objects"
                    ${RAPIDJSON_INCLUDE_DIRS}
)

add_executable(${PROJ_NAME} koala.cpp)
target_link_libraries(${PROJ_NAME} PRIVATE
    KoalaEngine
    Magnum::Application
    Magnum::GL
    Magnum::Magnum)

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})
