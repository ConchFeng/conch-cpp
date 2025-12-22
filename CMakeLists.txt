cmake_minimum_required(VERSION 3.25)

project(ConchCrossPlatform LANGUAGES CXX C)

# --- Global Settings ---
# Enable C++26 support (requires: MSVC 17.9+, GCC 14+, Clang 18+)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Standardize output directories (bin/ and lib/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Conan Integration ---
# Conan-generated files are typically in the build/ directory
find_package(spdlog REQUIRED)
find_package(libuv REQUIRED)
find_package(fmt REQUIRED)
find_package(benchmark REQUIRED)
find_package(GTest REQUIRED)

# Qt is only required for desktop apps, not mobile
if(NOT (CMAKE_SYSTEM_NAME STREQUAL "iOS" OR CMAKE_SYSTEM_NAME STREQUAL "Android"))
    find_package(Qt6 COMPONENTS Core Gui Widgets Network REQUIRED)
endif()

# --- Add Subdirectories ---
add_subdirectory(src/libs)
add_subdirectory(src/servers)
add_subdirectory(src/apps)
add_subdirectory(tests)
# add_subdirectory(tools) # Optional, can be enabled later
