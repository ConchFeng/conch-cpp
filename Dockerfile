# Multi-stage Docker build for Conch Cross-Platform Project
# Stage 1: Build environment
FROM ubuntu:22.04 AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    wget \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Install Conan
RUN pip3 install conan

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Install Conan dependencies
RUN conan install . --build=missing

# Build the project
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --config Release -j$(nproc)

# Stage 2: Runtime environment (minimal)
FROM ubuntu:22.04 AS runtime

# Install only runtime dependencies
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user
RUN useradd -m -u 1000 conchuser

# Copy binaries from builder
COPY --from=builder /app/build/bin/* /usr/local/bin/

# Switch to non-root user
USER conchuser

# Default command
CMD ["trading_engine"]
