project(CAN_C_API_Example)
cmake_minimum_required(VERSION 3.20)
set(SOURCES 
    src/CAN_Signal_Single_Point_Output.c
    )


set(EXTERNAL_INCLUDES
    "c:/Program Files (x86)/National Instruments/Shared/CVI/Include")
set(NIXNET_LIB "${CMAKE_SOURCE_DIR}/lib/nixnet.lib")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${EXTERNAL_INCLUDES}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${NIXNET_LIB}
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/lib/nixnet.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
# cmake -G "Ninja" .. ; cmake --build .