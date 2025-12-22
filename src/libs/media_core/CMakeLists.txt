add_library(media_core)

target_sources(media_core PRIVATE
    src/signalling.cpp
)

target_include_directories(media_core PUBLIC
    include
)

target_link_libraries(media_core PUBLIC
    foundation
    network
    fmt::fmt
)
