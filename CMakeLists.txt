cmake_minimum_required(VERSION 3.10.0)
project(industrial_robotics VERSION 0.1.0 LANGUAGES C CXX)

set(OpenCV_DIR
    "D:\\opencccc\\vscv\\install\\x64\\mingw\\lib"
)
find_package(OpenCV REQUIRED)

add_executable(industrial_robotics 
    src/main.cpp
    src/process.cpp
)

target_include_directories(industrial_robotics
    PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(industrial_robotics
    ${OpenCV_LIBS}
)
