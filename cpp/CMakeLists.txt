cmake_minimum_required(VERSION 3.21)

project(cpp)

set(__CT_MEMORY_DEBUG__ 1)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/output)
set(SOURCES src/main.cpp src/App.cpp src/Drawer.cpp  src/Particule.cpp src/PartManager.cpp src/SDLManagment.cpp)

set(CMAKE_CXX_STANDARD 14)
include_directories(include)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})
add_executable(fireball ${SOURCES})
target_link_libraries(fireball ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})
