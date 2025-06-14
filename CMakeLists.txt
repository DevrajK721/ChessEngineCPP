cmake_minimum_required(VERSION 3.15)
project(ChessEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Where to find headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Engine library
file(GLOB ENGINE_SOURCES src/*.cpp)
add_library(engine ${ENGINE_SOURCES})

# Executable
add_executable(chess-cli src/main.cpp)
target_link_libraries(chess-cli PRIVATE engine)

# Tests (optional, if you add tests later)
# enable_testing()
# add_subdirectory(tests)
