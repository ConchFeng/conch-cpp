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

# Auto-detect system settings (OS, compiler) and generate a default profile.
# The --force flag ensures it overwrites any existing profile to avoid errors.
conan profile detect --force

CONAN_ARGS=""

# [MacOS] Suppress 'unguarded-availability' errors triggered by recent SDK updates.
if [[ "$(uname)" == "Darwin" ]]; then
    echo "macOS detected - Applying Xcode 16.4+ compatibility fix..."
    CONAN_ARGS="$CONAN_ARGS -c tools.build:cflags+=[\"-Wno-error=unguarded-availability-new\"]"
    CONAN_ARGS="$CONAN_ARGS -c tools.build:cxxflags+=[\"-Wno-error=unguarded-availability-new\"]"
fi

# Install dependencies with Conan
echo "📦 Installing dependencies with Conan..."
conan install .. \
  --build=missing \
  -c tools.system.package_manager:mode=install \
  -c tools.system.package_manager:sudo=True \
  $CONAN_ARGS

# Configure CMake with Conan toolchain
echo "🔧 Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake

# Build
echo "🔨 Building project..."
cmake --build . --config Release -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "✅ Build complete! Binaries are in build/bin/"
