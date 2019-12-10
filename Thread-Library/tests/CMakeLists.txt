cmake_minimum_required(VERSION 3.0.0)
project (SpindleTest)
add_executable(spindleTest main.cpp)
TARGET_LINK_LIBRARIES( spindleTest SpindleLib)