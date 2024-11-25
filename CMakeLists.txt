# Minimum CMake version required
cmake_minimum_required(VERSION 3.15)

# Project name
project(8puzzle)

# Specify the target executable and library names
set(TARGET_8PUZZLE 8puzzle)
set(LIB_8PUZZLE 8puzzle_lib)
set(TEST_8PUZZLE test_8puzzle)

# Source files
set(SRC_8PUZZLE uni-fmi/ai/8puzzle/solution.cpp)
set(MAIN_SRC uni-fmi/ai/8puzzle/main.cpp)
set(TEST_SRC uni-fmi/ai/8puzzle/test.cpp)
set(HEADER_8PUZZLE uni-fmi/ai/8puzzle/solution.h)

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

# Add a library for the 8puzzle solution
add_library(${LIB_8PUZZLE} STATIC ${SRC_8PUZZLE} ${HEADER_8PUZZLE})
# If you want a shared library instead, replace STATIC with SHARED in the line above

# Add include directories for the library
target_include_directories(${LIB_8PUZZLE} PUBLIC uni-fmi/ai/8puzzle)

# Add the main executable and link it with the 8puzzle library
add_executable(${TARGET_8PUZZLE} ${MAIN_SRC})
target_link_libraries(${TARGET_8PUZZLE} PRIVATE ${LIB_8PUZZLE})

# Add the test executable and link it with the 8puzzle library
add_executable(${TEST_8PUZZLE} ${TEST_SRC})
target_link_libraries(${TEST_8PUZZLE} PRIVATE ${LIB_8PUZZLE})
