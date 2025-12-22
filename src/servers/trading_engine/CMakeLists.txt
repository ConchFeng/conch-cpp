add_executable(trading_engine main.cpp)

target_link_libraries(trading_engine PRIVATE
    foundation
    network
    quant_core
    spdlog::spdlog
    fmt::fmt
)
