#!/bin/bash
# Build script for Conch Cross-Platform Project

set -e

echo "🚀 Starting Conch Cross-Platform Build..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "📁 Creating build directory..."
    mkdir -p build
fi

cd build

# Install dependencies with Conan
echo "📦 Installing dependencies with Conan..."
conan install .. --build=missing

# Configure CMake
echo "🔧 Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo "🔨 Building project..."
cmake --build . --config Release -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "✅ Build complete! Binaries are in build/bin/"
