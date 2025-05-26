cmake_minimum_required(VERSION 3.28)
project(TetrisMultiWindow LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Pobierz SFML
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Dodaj pliki źródłowe
add_executable(Tetris
    src/main.cpp
    src/App.cpp
    src/MainMenu.cpp
    src/LevelSelect.cpp
    src/Game.cpp
    src/Tetromino.cpp
    src/Field.cpp
)

# Dodaj pliki nagłówkowe do include path
target_include_directories(Tetris PRIVATE src)

# Linkuj z SFML
target_link_libraries(Tetris PRIVATE sfml-graphics sfml-window sfml-system)

# Dla Windows — kopiowanie DLL
if(WIN32)
    add_custom_command(
        TARGET Tetris
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
            $<TARGET_FILE_DIR:Tetris>
        VERBATIM
    )
endif()
