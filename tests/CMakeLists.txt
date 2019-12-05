project(Koala)

find_package(Magnum REQUIRED GL Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Add Debuglogger to Koala
include_directories("../src/debuglogger")
include_directories("../src/engine")

add_executable(Koala koala.cpp)
target_link_libraries(Koala PRIVATE
    KoalaEngine
    Magnum::Application
    Magnum::GL
    Magnum::Magnum)

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Koala)
