#include <uv.h>
#include <spdlog/spdlog.h>
#include <fmt/core.h>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

uv_loop_t* loop;
uv_tcp_t client;
std::atomic<bool> running{true};

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0) {
        if (nread != UV_EOF) {
            spdlog::error("Read error: {}", uv_strerror(nread));
        }
        spdlog::info("Disconnected from server");
        running = false;
        uv_close((uv_handle_t*)stream, nullptr);
        delete[] buf->base;
        return;
    }
    
    if (nread > 0) {
        std::string msg(buf->base, nread);
        std::cout << msg << std::flush;
    }
    
    delete[] buf->base;
}

void on_connect(uv_connect_t* req, int status) {
    if (status < 0) {
        spdlog::error("Connection failed: {}", uv_strerror(status));
        running = false;
        delete req;
        return;
    }
    
    spdlog::info("Connected to chat server!");
    fmt::print("\n=== Connected to Chat Server ===\n");
    fmt::print("Type your messages and press Enter to send.\n");
    fmt::print("Type 'quit' to exit.\n");
    fmt::print("================================\n\n");
    
    uv_read_start(req->handle, alloc_buffer, on_read);
    delete req;
}

void stdin_reader() {
    std::string line;
    while (running && std::getline(std::cin, line)) {
        if (line == "quit" || line == "exit") {
            running = false;
            uv_stop(loop);
            break;
        }
        
        if (!line.empty()) {
            line += "\n";
            uv_buf_t buf = uv_buf_init(const_cast<char*>(line.c_str()), 
                                      line.length());
            uv_write_t* req = new uv_write_t;
            uv_write(req, (uv_stream_t*)&client, &buf, 1, 
                [](uv_write_t* req, int status) {
                    if (status < 0) {
                        spdlog::error("Write error: {}", uv_strerror(status));
                    }
                    delete req;
                });
        }
    }
}

int main(int argc, char** argv) {
    spdlog::info("Starting Chat Client...");
    
    const char* host = "127.0.0.1";
    int port = 8888;
    
    if (argc >= 2) host = argv[1];
    if (argc >= 3) port = std::atoi(argv[2]);
    
    loop = uv_default_loop();
    
    uv_tcp_init(loop, &client);
    
    struct sockaddr_in dest;
    uv_ip4_addr(host, port, &dest);
    
    uv_connect_t* connect = new uv_connect_t;
    uv_tcp_connect(connect, &client, (const struct sockaddr*)&dest, on_connect);
    
    fmt::print("Connecting to {}:{}...\n", host, port);
    
    // Start input thread
    std::thread input_thread(stdin_reader);
    
    // Run event loop
    uv_run(loop, UV_RUN_DEFAULT);
    
    running = false;
    if (input_thread.joinable()) {
        input_thread.join();
    }
    
    spdlog::info("Chat client terminated");
    return 0;
}
