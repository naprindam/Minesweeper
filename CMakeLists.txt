cmake_minimum_required(VERSION 3.15)

project(Minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SFML
    SOURCE_DIR "D:/下载/SFML-2.6.2/SFML-2.6.2"
)

FetchContent_MakeAvailable(SFML)

add_executable(Minesweeper
    src/main.cpp
    src/Board.cpp
)

target_link_libraries(Minesweeper PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)