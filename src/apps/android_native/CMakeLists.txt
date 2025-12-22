project(android_native)

# Android native application (JNI library)
# This builds a shared library (.so) that can be called from Android Java/Kotlin layer

add_library(${PROJECT_NAME} SHARED
    jni/app_jni.cpp
    src/app_core.cpp
    include/android_native/app_core.h
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        foundation
        network
        media_core
        log  # Android log library
)

# Android-specific settings
if(ANDROID)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ANDROID_PLATFORM)
endif()
