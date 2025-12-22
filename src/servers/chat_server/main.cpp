#include <uv.h>
#include <spdlog/spdlog.h>
#include <fmt/core.h>
#include <vector>
#include <string>
#include <cstring>

struct client_t {
    uv_tcp_t handle;
    std::string name;
};

std::vector<client_t*> clients;
uv_loop_t* loop;

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void broadcast_message(client_t* sender, const char* msg, size_t len) {
    for (auto* client : clients) {
        if (client != sender) {
            uv_buf_t buf = uv_buf_init(const_cast<char*>(msg), len);
            uv_write_t* req = new uv_write_t;
            uv_write(req, (uv_stream_t*)&client->handle, &buf, 1, 
                [](uv_write_t* req, int status) { delete req; });
        }
    }
}

void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    client_t* client = (client_t*)stream->data;
    
    if (nread < 0) {
        if (nread != UV_EOF) {
            spdlog::error("Read error: {}", uv_strerror(nread));
        }
        
        spdlog::info("Client {} disconnected", client->name);
        std::string leave_msg = fmt::format("[Server] {} left the chat\n", client->name);
        broadcast_message(client, leave_msg.c_str(), leave_msg.length());
        
        // Remove from clients list
        clients.erase(std::remove(clients.begin(), clients.end(), client), 
                     clients.end());
        
        uv_close((uv_handle_t*)stream, [](uv_handle_t* handle) {
            delete (client_t*)handle->data;
        });
        delete[] buf->base;
        return;
    }
    
    if (nread > 0) {
        std::string msg(buf->base, nread);
        spdlog::info("[{}]: {}", client->name, msg.substr(0, msg.length()-1));
        
        std::string broadcast = fmt::format("[{}]: {}", client->name, msg);
        broadcast_message(client, broadcast.c_str(), broadcast.length());
    }
    
    delete[] buf->base;
}

void on_new_connection(uv_stream_t* server, int status) {
    if (status < 0) {
        spdlog::error("Connection error: {}", uv_strerror(status));
        return;
    }
    
    client_t* client = new client_t;
    uv_tcp_init(loop, &client->handle);
    client->handle.data = client;
    
    if (uv_accept(server, (uv_stream_t*)&client->handle) == 0) {
        static int client_id = 0;
        client->name = fmt::format("User{}", ++client_id);
        
        clients.push_back(client);
        spdlog::info("New client connected: {}", client->name);
        
        std::string welcome = fmt::format("[Server] Welcome {}! Type messages to chat.\n", 
                                         client->name);
        uv_buf_t buf = uv_buf_init(const_cast<char*>(welcome.c_str()), 
                                   welcome.length());
        uv_write_t* req = new uv_write_t;
        uv_write(req, (uv_stream_t*)&client->handle, &buf, 1, 
            [](uv_write_t* req, int status) { delete req; });
        
        std::string join_msg = fmt::format("[Server] {} joined the chat\n", client->name);
        broadcast_message(client, join_msg.c_str(), join_msg.length());
        
        uv_read_start((uv_stream_t*)&client->handle, alloc_buffer, on_read);
    } else {
        uv_close((uv_handle_t*)&client->handle, [](uv_handle_t* handle) {
            delete (client_t*)handle->data;
        });
    }
}

int main() {
    spdlog::info("Starting Chat Server...");
    
    loop = uv_default_loop();
    
    uv_tcp_t server;
    uv_tcp_init(loop, &server);
    
    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 8888, &addr);
    
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    
    int r = uv_listen((uv_stream_t*)&server, 128, on_new_connection);
    if (r) {
        spdlog::error("Listen error: {}", uv_strerror(r));
        return 1;
    }
    
    spdlog::info("Chat server listening on port 8888");
    fmt::print("Chat Server is running on port 8888\n");
    fmt::print("Clients can connect using: ./run.sh chat_client\n");
    
    return uv_run(loop, UV_RUN_DEFAULT);
}
