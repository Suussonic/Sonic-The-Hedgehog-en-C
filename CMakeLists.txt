cmake_minimum_required(VERSION 3.28)
project(Sonic C)

set(CMAKE_C_STANDARD 11)

set(SDL_INCLUDE_DIR "C:/msys64/mingw64/include/SDL")
set(SDL_IMAGE_INCLUDE_DIR SDL_INCLUDE_DIR)

set(SDL_LIBRARY "C:/msys64/mingw64/lib/libSDL.a")
set(SDL_IMAGE_LIBRARY "C:/msys64/mingw64/lib/libSDL_image.a")

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)

if (SDL_FOUND)
    include_directories(${SDL_INCLUDE_DIR})
    add_executable(Sonic main.c)
    target_link_libraries(Sonic ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARY})
else ()
    message(FATAL_ERROR "SDL 1.2 not found. Please install SDL 1.2.")
endif ()
