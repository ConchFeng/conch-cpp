#!/bin/bash
# Run script for Conch Cross-Platform Project executables

set -e

# Check if build directory exists
if [ ! -d "build/bin" ]; then
    echo "❌ Build directory not found. Please run ./build.sh first."
    exit 1
fi

# Function to display usage
usage() {
    echo "Usage: ./run.sh [executable_name]"
    echo ""
    echo "Available executables:"
    for exe in build/bin/*; do
        if [ -f "$exe" ] && [ -x "$exe" ]; then
            echo "  - $(basename $exe)"
        fi
    done
    exit 1
}

# Check if argument provided
if [ -z "$1" ]; then
    usage
fi

EXECUTABLE="build/bin/$1"

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "❌ Executable '$1' not found in build/bin/"
    usage
fi

# Run the executable
echo "🏃 Running $1..."
$EXECUTABLE "${@:2}"
