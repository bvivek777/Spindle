cmake_minimum_required(VERSION 3.10)

# set the project name
project(Spindle VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include the files
include_directories(Include)
file(GLOB SOURCES "Src/*.cpp")

# add the executable
add_executable(Spindle ${SOURCES})