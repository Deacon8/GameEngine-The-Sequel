cmake_minimum_required(VERSION 3.10)

project(3DBrowser)

set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include/SDL2")
set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2Lib/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2Lib/SDL2main.lib")

add_executable(3DBrowser src/main.cpp src/glad.c src/shader.cpp src/memory.cpp)
target_include_directories(3DBrowser PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(3DBrowser ${SDL2_LIBRARIES})