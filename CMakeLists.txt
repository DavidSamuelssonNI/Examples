cmake_minimum_required(VERSION 3.20)
project(CAN_C_API_Example)

# Include the cross-compilation toolchain file
include(toolchain.cmake)

# Ensure CMake recognizes the system as Linux
set(CMAKE_SYSTEM_NAME Linux)

# Clear any Windows linker flags
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_MODULE_LINKER_FLAGS "")
set(CMAKE_SHARED_LINKER_FLAGS "")

# Set sources and library
set(SOURCES 
    src/main_CAN_loopback.c
    src/session.c
    src/can_signal_single_point_out.c
)
set(NIXNET_LIB lib/libnixnet.so)

# Define executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${NIXNET_LIB})

# Remove .exe suffix and set Linux-compatible output name
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "CAN_C_API_Example" SUFFIX "")

# cmake -G "Ninja" .. ; cmake --build .