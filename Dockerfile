# Multi-stage Docker build for Conch Cross-Platform Project
# Stage 1: Build environment
FROM ubuntu:22.04 AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    python3 \
    python3-pip \
    wget \
    curl \
    libgl1-mesa-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Install Conan
RUN pip3 install conan

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Create build directory and navigate into it
RUN mkdir -p build

# Create Conan default profile
RUN conan profile detect --force

# Install Conan dependencies with same config as build.sh
WORKDIR /app/build
RUN conan install .. \
    --build=missing \
    -c tools.system.package_manager:mode=install \
    -c tools.system.package_manager:sudo=True

# Configure CMake with Conan toolchain (matching build.sh)
RUN cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake

# Build the project
RUN cmake --build . --config Release -j$(nproc)

# Stage 2: Runtime environment (minimal)
FROM ubuntu:22.04 AS runtime

# Install only runtime dependencies
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    libgl1 \
    libglib2.0-0 \
    libdbus-1-3 \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user
RUN useradd -m -u 1000 conchuser

# Copy binaries from builder
COPY --from=builder /app/build/bin/* /usr/local/bin/

# Switch to non-root user
USER conchuser

# Default command
CMD ["trading_engine"]
