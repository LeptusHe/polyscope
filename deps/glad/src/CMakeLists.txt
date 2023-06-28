cmake_minimum_required(VERSION 3.1)

# Create a library for the viewer code
add_library(
    polyscope_glad
    glad.c
    )

# Are we building a shared library?
get_target_property(library_type polyscope_glad TYPE)
if (library_type STREQUAL SHARED_LIBRARY)
  # If so, define the macro GLAD_API_EXPORT on the command line.
  target_compile_definitions(polyscope_glad PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD)
endif()

target_include_directories(polyscope_glad PRIVATE "../include")

set_target_properties(polyscope_glad PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
add_library(polyscope::glad ALIAS polyscope_glad)