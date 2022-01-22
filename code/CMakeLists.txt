cmake_minimum_required(VERSION 3.15)
project(fran VERSION 1.0 DESCRIPTION "testing application")

add_subdirectory(dep/DPP)
add_subdirectory(dep/spdlog)

add_executable(${fran}
    src/fran.cpp
    src/fran.h
)
 
target_link_libraries(${fran}
    dpp
    spdlog 
)
 
target_include_directories(${fran} PRIVATE
    dep/DPP/include
    dep/spdlog/include
)
 
set_target_properties(${fran} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
