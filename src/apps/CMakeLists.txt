# CMake configuration for apps directory
# Desktop: Qt-based cross-platform app
# Mobile: Native iOS and Android apps

# Only build Qt apps if Qt6 is available
if(Qt6_FOUND)
    message(STATUS "Qt6 found - building Qt applications")
    add_subdirectory(desktop_pro)
    add_subdirectory(snake_qt)    # Qt Snake game
    add_subdirectory(breakout_qt)
    add_subdirectory(mario_qt)    # Super Mario platform game
else()
    message(STATUS "Qt6 not found - skipping Qt applications")
endif()

add_subdirectory(snake_cli)   # CLI Snake game
add_subdirectory(chat_client) # Chat client
add_subdirectory(stock_trader) # Stock trading simulator
add_subdirectory(ios_native)
add_subdirectory(httpserver_examples) # HTTP server framework examples
add_subdirectory(rtc_sdk_demo) # RTC SDK demonstration

# Android app requires Android NDK
if(ANDROID)
    add_subdirectory(android_native)
endif()
