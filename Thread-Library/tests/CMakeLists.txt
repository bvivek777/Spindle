cmake_minimum_required(VERSION 3.0.0)
project(Spindle_Test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

get_filename_component(DIR_ONE_ABOVE ../ ABSOLUTE)

set(Include_Spindle_Dir DIR_ONE_ABOVE)

add_subdirectory(WorkSpace)


include_directories(${Include_Spindle_Dir})

add_executable(Test main.cpp)

target_link_libraries(Test WorkSpace)
ADD_DEPENDENCIES(Test WorkSpace)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
