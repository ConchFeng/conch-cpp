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
            echo "  - $(basename "$exe")"
        elif [ -d "$exe" ] && [[ "$(basename "$exe")" == *.app ]]; then
            # macOS .app bundle
            echo "  - $(basename "$exe" .app)"
        fi
    done
    exit 1
}

# Check if argument provided
if [ -z "$1" ]; then
    usage
fi

EXEC_NAME="$1"

# Check for regular executable
if [ -f "build/bin/$EXEC_NAME" ] && [ -x "build/bin/$EXEC_NAME" ]; then
    echo "🏃 Running $EXEC_NAME..."
    "build/bin/$EXEC_NAME" "${@:2}"
    exit 0
fi

# Check for macOS .app bundle
if [ -d "build/bin/${EXEC_NAME}.app" ]; then
    echo "🏃 Running ${EXEC_NAME}.app..."
    open "build/bin/${EXEC_NAME}.app"
    exit 0
fi

# Not found
echo "❌ Executable '$EXEC_NAME' not found in build/bin/"
usage
