#!/bin/bash
# Documentation generation script using Doxygen

set -e

echo "========================================"
echo "Generating API Documentation"
echo "========================================"

# Check if Doxygen is installed
if ! command -v doxygen &> /dev/null; then
    echo "❌ Doxygen not found. Please install it:"
    echo "   Ubuntu/Debian: sudo apt-get install doxygen graphviz"
    echo "   macOS: brew install doxygen graphviz"
    echo "   Windows: Download from https://www.doxygen.nl/download.html"
    exit 1
fi

echo ""
echo "[1/2] Running Doxygen..."
doxygen Doxyfile

echo ""
echo "[2/2] Documentation generated!"
echo ""
echo "HTML docs: docs/html/index.html"
echo "LaTeX docs: docs/latex/"
echo ""
echo "To view documentation:"
echo "  open docs/html/index.html        # macOS"
echo "  xdg-open docs/html/index.html    # Linux"
echo "  start docs/html/index.html       # Windows"
echo ""
echo "✅ Documentation generation complete!"
