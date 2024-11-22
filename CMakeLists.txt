cmake_minimum_required(VERSION 3.0)

project(randomLibrary)

set(DEFINES kNIOSLinux)
set(DEBUG_MODE 0)

if (DEBUG_MODE)
    add_compile_options(-g)
else ()
    add_compile_options(-Os -fstrength-reduce -fno-builtin -fno-strict-aliasing)
endif ()

include_directories(include)

set(SOURCES
    src/random.c
    src/ni_modelframework.c
)

# Create the shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Set the output name of the shared library
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    PREFIX "lib"

)
target_compile_options(${PROJECT_NAME} PRIVATE -MD -MP -fdollars-in-identifiers -Wall -fPIC)

# Define preprocessor macros
target_compile_definitions(${PROJECT_NAME} PRIVATE -D${DEFINES})

