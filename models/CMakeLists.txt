cmake_minimum_required(VERSION 3.20)
include(../toolchain.cmake)

project(mymodel)

# Set sources and library
set(SOURCES 
    mymodel.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND scp -C $<TARGET_FILE:${PROJECT_NAME}> admin@192.168.0.3:/home/Temp
    COMMENT "Copying executable to remote server"
)