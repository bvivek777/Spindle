cmake_minimum_required(VERSION 3.0.0)
project(Spindle VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
file(GLOB SOURCES src/*.cpp)

add_library(SpindleLib STATIC ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
