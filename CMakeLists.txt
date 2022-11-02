cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(game)

set(LUA_VERSION 5.4.1)
set(RAYLIB_VERSION 4.2.0)

FILE(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib lua)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC src)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()