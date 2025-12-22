#include "media/signalling.hpp"
#include <fmt/core.h>

namespace media {
    void handle_signal(const std::string& msg) {
        fmt::print("Received signal: {}\\n", msg);
    }
}
