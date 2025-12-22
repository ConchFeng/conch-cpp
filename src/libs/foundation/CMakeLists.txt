project(foundation)

# Create library
add_library(${PROJECT_NAME} 
    src/logger.cpp 
    include/foundation/logger.h
)

# Include paths
target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Link dependencies (PUBLIC means apps using foundation can also use spdlog directly)
target_link_libraries(${PROJECT_NAME} 
    PUBLIC spdlog::spdlog
)

# Enable modern compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()
