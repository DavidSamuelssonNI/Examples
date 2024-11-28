cmake_minimum_required(VERSION 3.20)
include(toolchain.cmake)
project(CAN_C_API_Example)

# Set sources and library
set(SOURCES 
    src/main_CAN_loopback.c
    src/session.c
    # src/can_signal_single_point_out.c
)
# move into C:\build\2024Q4\x64\sysroots\core2-64-nilrt-linux\usr\lib\ and automatically take from cmake file?
#or add all libs here?
set(NIXNET_LIB 
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixnet.so
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixnetDB.so.1
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixntdm.so.1
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixnthc.so.2
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixntpi.so.1
    # ${CAN_C_API_Example_SOURCE_DIR}/lib/libnixnttcpip.so.23
    ${CAN_C_API_Example_SOURCE_DIR}/lib/libxnetTrace.so.1
    ) 

# Define executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CAN_C_API_Example_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${NIXNET_LIB} pthread)

# Add custom command to run scp after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND scp $<TARGET_FILE:${PROJECT_NAME}> admin@192.168.0.3:/home/Temp
    COMMENT "Copying executable to remote server"
)

# cmake -G "Ninja" .. ; cmake --build .