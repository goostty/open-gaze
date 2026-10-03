cmake_minimum_required(VERSION 3.10)
project(FaceBreak)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXPORT_COMPILE_COMMANDS TRUE)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(src)

add_executable(open_gaze src/main.cpp src/vision/vision.cpp)
target_link_libraries(open_gaze ${OpenCV_LIBS})
