cmake_minimum_required(VERSION 3.15)
project(CacheObliviousLZ C)
# -----------------------------------------
# Detect OS
# -----------------------------------------
message(STATUS "Operating System: ${CMAKE_SYSTEM_NAME}")
# -----------------------------------------
# Detect CPU architecture
# -----------------------------------------
message(STATUS "Processor Architecture: ${CMAKE_SYSTEM_PROCESSOR}")
# Normalize architecture names by making them all lowercase
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH_LOWER)
#  Architecture naming variations collapse into one label that the build system can work off of
if(ARCH_LOWER MATCHES "x86_64|amd64")
    set(ARCH "x86_64")
elseif(ARCH_LOWER MATCHES "arm64|aarch64")
    set(ARCH "arm64")
elseif(ARCH_LOWER MATCHES "riscv64|risc-v")
    set(ARCH "riscv64")
else()
    set(ARCH "unknown")
endif()
# Detect architecture (standardized)
message(STATUS "Detected Architecture: ${ARCH}")
# -----------------------------------------
# Compiler flags for optimization
# -----------------------------------------
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-O3 -Wall -Wextra -pedantic)
elseif (MSVC)
    add_compile_options(/O2 /W3)
endif()
# -----------------------------------------
# Source files
# NEED TO BE ADJUSTED
# -----------------------------------------
set(SOURCES
    main.c
    cache_naive_helpers.c
)
# -----------------------------------------
# Executable target
# -----------------------------------------
add_executable(image_compressor
    main.c
    cache_naive_helpers.c
)
target_link_libraries(image_compressor m)
# No libs needed for stb_image (header-only)
target_include_directories(image_compressor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# -----------------------------------------
# Print summary
# -----------------------------------------
message(STATUS "---------------------------------------")
message(STATUS "Build Summary")
message(STATUS "  OS:        ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Arch:      ${ARCH}")
message(STATUS "  Compiler:  ${CMAKE_C_COMPILER}")
message(STATUS "---------------------------------------")
