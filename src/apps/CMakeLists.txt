# CMake configuration for apps directory
# Desktop: Qt-based cross-platform app
# Mobile: Native iOS and Android apps

add_subdirectory(desktop_pro)

# CLI Snake requires Unix terminal APIs (termios.h), skip on Windows
if(NOT WIN32)
    add_subdirectory(snake_cli)   # CLI Snake game (Unix/Linux/macOS only)
endif()

add_subdirectory(snake_qt)    # Qt Snake game
add_subdirectory(chat_client) # Chat client
add_subdirectory(stock_trader) # Stock trading simulator

# iOS app only builds on iOS platform
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    add_subdirectory(ios_native)
endif()

# Android app requires Android NDK
if(ANDROID)
    add_subdirectory(android_native)
endif()
