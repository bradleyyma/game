cmake_minimum_required(VERSION 3.16)
project(mygame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

message(STATUS "SDL2 found: ${SDL2_FOUND}")
message(STATUS "SDL2_image found: ${SDL2_IMAGE_FOUND}")
message(STATUS "SDL2_ttf found: ${SDL2_TTF_FOUND}")

add_executable(my_game 
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Monster.cpp
    src/Slime.cpp
    src/MonsterFactory.cpp
    src/GameUtils.cpp
)
target_link_libraries(my_game PRIVATE SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2::SDL2 SDL2_ttf::SDL2_ttf)
