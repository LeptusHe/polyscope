cmake_minimum_required(VERSION 3.1)

# Create a library for the viewer code
add_library(
    polyscope-stb
    stb_impl.cpp
    )

target_include_directories(stb PRIVATE "../include")    

set_target_properties(polyscope-stb PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
