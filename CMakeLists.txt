cmake_minimum_required(VERSION 3.10)
project(PlaneGame VERSION 1.0 LANGUAGES C CXX) 

set(TARGET ${PROJECT_NAME}-${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

if(MSVC)
    add_compile_options(/W4 "/source-charset:utf-8")
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(SDL2_DIR "D:/GCC/Library/SDL2/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED) 
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${TARGET} "src/main.cpp" "src/Game.cpp" "src/Scene_main.cpp")

target_link_libraries(${TARGET} 
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image 
    SDL2_ttf::SDL2_ttf 
    SDL2_mixer::SDL2_mixer
)