cmake_minimum_required(VERSION 3.20)
include(toolchain.cmake)
project(CAN_C_API_Example)

# Set sources and library
set(SOURCES 
    src/main_CAN_loopback.c
    src/session.c
    src/can_signal_single_point_out.c
)
set(NIXNET_LIB "C:/Temp/sw_dev/Examples/lib/libnixnet.so")

# Define executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE "C:/Temp/sw_dev/Examples/include")

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${NIXNET_LIB})


# cmake -G "Ninja" .. ; cmake --build .