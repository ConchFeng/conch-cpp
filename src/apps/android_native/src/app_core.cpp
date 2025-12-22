#include "android_native/app_core.h"
#include <spdlog/spdlog.h>

namespace android_native {
    void initialize_app() {
        spdlog::info("Android Native App Initialized");
    }
    
    void start_meeting() {
        spdlog::info("Starting Android Meeting...");
    }
}
