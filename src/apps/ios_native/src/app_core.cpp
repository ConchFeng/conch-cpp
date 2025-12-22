#include "ios_native/app_core.h"
#include <spdlog/spdlog.h>

namespace ios_native {
    void initialize_app() {
        spdlog::info("iOS Native App Initialized");
    }
    
    void start_meeting() {
        spdlog::info("Starting iOS Meeting...");
    }
}
