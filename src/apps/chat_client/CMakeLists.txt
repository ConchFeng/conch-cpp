project(chat_client)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        foundation
        spdlog::spdlog
        fmt::fmt
        libuv::uv_a
)
