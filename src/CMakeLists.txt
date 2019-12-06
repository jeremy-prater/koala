project(KoalaEngine)

find_package(RapidJSON)
find_package(Magnum REQUIRED GL Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Setup include directories
include_directories("debuglogger"
                    "engine"
                    )

# Add Debug logger
set(DEBUGLOGGER_SRCS "debuglogger/debuglogger.cpp")

# Add Koala engine
set(KOALAENGINE_SRCS "engine/koala-engine.cpp") 

set(SOURCE_FILES ${DEBUGLOGGER_SRCS} ${KOALAENGINE_SRCS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(KoalaEngine STATIC ${SOURCE_FILES})
target_link_libraries(KoalaEngine PRIVATE
    Magnum::Application
    Magnum::GL
    Magnum::Magnum)