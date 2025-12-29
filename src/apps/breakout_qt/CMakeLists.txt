project(breakout_qt)

# Enable Qt automatic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME}
    main.cpp
    game_widget.cpp
    game_widget.h
    gl_game_widget.cpp
    gl_game_widget.h
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        foundation
        spdlog::spdlog
        Qt6::Widgets
        Qt6::Core
        Qt6::OpenGL
        Qt6::OpenGLWidgets
)

# macOS app bundle settings
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()

# Set C++ standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
