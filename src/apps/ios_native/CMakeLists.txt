project(ios_native)

# iOS native application
# This builds a static library that can be linked with iOS UI layer (Swift/Objective-C)

add_library(${PROJECT_NAME} STATIC
    src/app_core.cpp
    include/ios_native/app_core.h
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        foundation
        network
        media_core
)

# iOS-specific compile flags
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER com.conch.iosnative
    )
endif()
