project(mario_qt)

# Enable Qt automatic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME}
    main.cpp
    mario_game_widget.cpp
    mario_game_widget.h
    physics_engine.cpp
    physics_engine.h
    level_data.cpp
    level_data.h
    game_types.h
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
