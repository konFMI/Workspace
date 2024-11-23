# Minimum CMake version required
cmake_minimum_required(VERSION 3.15)

# Project name
project(8puzzle)

# Specify the target executable name
set(TARGET_8PUZZLE 8puzzle)

# Source files
set(SRC_8PUZZLE uni-fmi/ai/8puzzle/main.cpp)

# Find compilers
find_program(CLANG_PATH clang++)
find_program(GCC_PATH g++)
find_program(MSVC_PATH cl.exe)

# Check platform and select compiler
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Platform: Windows")

    if (MSVC_PATH)
        message(STATUS "MSVC found, using MSVC compiler")
        set(CXX_COMPILER_PATH ${MSVC_PATH})
        set(CMAKE_CXX_COMPILER ${MSVC_PATH})
        set(CMAKE_CXX_FLAGS "/W4 /EHsc")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /Od /DDEBUG /Zi")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /O2")
    else()
        # If no compiler is found, raise an error
        message(FATAL_ERROR "No compatible compiler found on Windows. Please install MSVC.")
    endif()

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Platform: Linux")

    if (GCC_PATH)
        message(STATUS "Using GCC on Linux")
        set(CXX_COMPILER_PATH ${GCC_PATH})
        set(CMAKE_CXX_COMPILER ${GCC_PATH})
        set(CMAKE_CXX_FLAGS "-Wall -g")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
    elseif (CLANG_PATH)
        message(STATUS "Using Clang on Linux")
        set(CXX_COMPILER_PATH ${CLANG_PATH})
        set(CMAKE_CXX_COMPILER ${CLANG_PATH})
        set(CMAKE_CXX_FLAGS "-Wall -g")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
    else()
        # If no compiler is found, raise an error
        message(FATAL_ERROR "No compatible compiler found on Linux. Please install Clang or GCC.")
    endif()
else()
    message(STATUS "Unknown platform: ${CMAKE_SYSTEM_NAME}")
    message(FATAL_ERROR "Unknown platform. CMake only supports Windows and Linux in this script.")
endif()

# Check if the compiler was successfully set
message(STATUS "Using compiler: ${CMAKE_CXX_COMPILER}")

# Add the executable
add_executable(${TARGET_8PUZZLE} ${SRC_8PUZZLE})
