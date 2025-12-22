#include "quant/model.hpp"
#include <cmath>

namespace quant {
    double calculate_option_price(double s, double k, double r, double v, double t) {
        // Placeholder Black-Scholes
        return s * std::exp(-r * t); // Simplified
    }
}
