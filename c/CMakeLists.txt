cmake_minimum_required(VERSION 3.21)

project(c)

set(__CT_MEMORY_DEBUG__ 1)
add_compile_definitions(__CT_MEMORY_DEBUG__=12)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/output)
set(SOURCES src/main.c src/affichage.c src/contexte.c src/ctools.c src/initSDL.c src/linkedList.c src/particule.c)

include_directories(include)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})
add_executable(fireball ${SOURCES})
target_link_libraries(fireball ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY})