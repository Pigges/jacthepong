cmake_minimum_required(VERSION 3.16)
project(ponggame LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(ponggame src/main.cpp
        src/Entity.cpp
        src/Entity.h
        src/Player.cpp
        src/Player.h
        src/Ball.cpp
        src/Ball.h
        src/RenderManager.cpp
        src/RenderManager.h
        src/Text.cpp
        src/Text.h
        src/ScoreManager.cpp
        src/ScoreManager.h
        src/Powerup.cpp
        src/Powerup.h)
target_link_libraries(ponggame PRIVATE sfml-graphics)
target_compile_features(ponggame PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
            TARGET ponggame
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:ponggame>
            VERBATIM)
endif()

install(TARGETS ponggame)