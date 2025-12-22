add_library(ui_kit)

# Qt6 is required
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

target_sources(ui_kit PRIVATE
    # src/custom_widget.cpp
)

target_link_libraries(ui_kit PUBLIC
    Qt6::Core Qt6::Gui Qt6::Widgets
    foundation
)
