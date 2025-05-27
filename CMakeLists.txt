cmake_minimum_required(VERSION 3.28)
project(TetrisMultiWindow LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# ===== SFML =====
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SFML)

# ===== SQLite3 lokalnie =====
# Zakładamy, że pliki sqlite3.c i sqlite3.h znajdują się w external/sqlite/
add_library(SQLite3 STATIC external/sqlite/sqlite3.c)
target_include_directories(SQLite3 PUBLIC external/sqlite)

# ===== Pliki źródłowe =====
add_executable(Tetris
    src/main.cpp
    src/App.cpp
    src/MainMenu.cpp
    src/LevelSelect.cpp
    src/Game.cpp
    src/Tetromino.cpp
    src/Field.cpp
    src/DatabaseManager.cpp
)

# ===== Font: skopiuj arial.ttf do katalogu bin =====
add_custom_command(
    TARGET Tetris
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/assets/fonts/arial.ttf
        $<TARGET_FILE_DIR:Tetris>
    COMMENT "Copying arial.ttf to output directory"
)

# ===== Ścieżki nagłówków =====
target_include_directories(Tetris PRIVATE src)

# ===== Linkowanie =====
target_link_libraries(Tetris
    PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        SQLite3
)

# ===== Windows: kopiowanie openal32.dll =====
if(WIN32)
    add_custom_command(
        TARGET Tetris
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
            $<TARGET_FILE_DIR:Tetris>
        COMMENT "Copying OpenAL DLL"
        VERBATIM
    )
endif()
