# CMake configuration for apps directory
# Desktop: Qt-based cross-platform app
# Mobile: Native iOS and Android apps

add_subdirectory(desktop_pro)
add_subdirectory(snake_cli)   # CLI Snake game
add_subdirectory(snake_qt)    # Qt Snake game
add_subdirectory(chat_client) # Chat client
add_subdirectory(ios_native)

# Android app requires Android NDK
if(ANDROID)
    add_subdirectory(android_native)
endif()
