add_library(quant_core)

target_sources(quant_core PRIVATE
    src/pricing_model.cpp
)

target_include_directories(quant_core PUBLIC
    include
)

target_link_libraries(quant_core PUBLIC
    foundation
    fmt::fmt
)
