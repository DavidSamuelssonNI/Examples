set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# root path for toolchains
set(toolchain_path "C:/build/2024Q4/x64/sysroots")

# compilers
set(CMAKE_C_COMPILER "${toolchain_path}/x86_64-w64-mingw32/usr/bin/x86_64-nilrt-linux/x86_64-nilrt-linux-gcc.exe")
set(CMAKE_CXX_COMPILER "${toolchain_path}/x86_64-w64-mingw32/usr/bin/x86_64-nilrt-linux/x86_64-nilrt-linux-g++.exe")

# sysroot
set(CMAKE_SYSROOT "${toolchain_path}/core2-64-nilrt-linux")

# compiler flags
set(CMAKE_C_FLAGS "--sysroot=${CMAKE_SYSROOT} -Wall -fmessage-length=0")
set(CMAKE_CXX_FLAGS "--sysroot=${CMAKE_SYSROOT} -Wall -fmessage-length=0")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# include directories
include_directories(
    ${CMAKE_SYSROOT}/usr
)

# find root path settings
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)