project(desktop_pro)

# Enable Qt automatic processing (MOC, UIC, RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

# Link internal libraries (foundation, network) and external libraries (Qt)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        foundation
        network
        Qt6::Widgets
        Qt6::Core
)

# For Windows GUI applications, prevent console window from appearing
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
endif()
