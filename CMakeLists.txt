cmake_minimum_required(VERSION 3.12)
project(perpetual_calendar_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(perpetual_calendar src/main.cpp)
