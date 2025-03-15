cmake_minimum_required(VERSION 3.15)
project(mygame)

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(SDL2_IMAGE_DIR /opt/homebrew/Cellar/sdl2_image/2.8.5)
set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_DIR}/include)
set(SDL2_IMAGE_LIBRARIES ${SDL2_IMAGE_DIR}/lib/libSDL2_image.dylib)

set(SDL2_TTF_DIR /opt/homebrew/Cellar/sdl2_ttf/2.24.0)
set(SDL2_TTF_INCLUDE_DIRS ${SDL2_TTF_DIR}/include)
set(SDL2_TTF_LIBRARIES ${SDL2_TTF_DIR}/lib/libSDL2_ttf.dylib)
# message(STATUS "SDL2_IMAGE_INCLUDE_DIRS: ${SDL2_DIR}")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)


message (STATUS "SDL2_image: ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_IMAGE_LIBRARIES}")


add_executable(my_game src/hello.cpp)
target_include_directories(my_game PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(my_game PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
