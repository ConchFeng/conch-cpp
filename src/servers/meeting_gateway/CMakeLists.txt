add_executable(meeting_gateway main.cpp)

target_link_libraries(meeting_gateway PRIVATE
    foundation
    network
    media_core
    spdlog::spdlog
    fmt::fmt
)
