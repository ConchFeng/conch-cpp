#!/bin/bash
# Automated test runner with coverage support

set -e

echo "========================================"
echo "Running Unit Tests & Benchmarks"
echo "========================================"

# Parse arguments
COVERAGE=false
if [[ "$1" == "--coverage" ]]; then
    COVERAGE=true
fi

# Ensure build directory exists
if [ ! -d "build" ]; then
    echo "❌ Build directory not found. Run ./build.sh first."
    exit 1
fi

cd build

echo ""
echo "[1/3] Running unit tests..."
if [ -f "bin/unit_tests" ]; then
    ./bin/unit_tests --gtest_output=xml:test-results/unit_tests.xml
    echo "✅ Unit tests passed!"
else
    echo "⚠️  Unit tests binary not found."
fi

echo ""
echo "[2/3] Running benchmarks..."
if [ -f "bin/bench_tests" ]; then
    ./bin/bench_tests --benchmark_out=test-results/benchmarks.json
    echo "✅ Benchmarks completed!"
else
    echo "⚠️  Benchmark binary not found."
fi

# Coverage reporting (Linux only)
if [ "$COVERAGE" = true ] && [ "$(uname)" = "Linux" ]; then
    echo ""
    echo "[3/3] Generating coverage report..."
    
    # Install gcovr if not present
    command -v gcovr >/dev/null 2>&1 || pip3 install gcovr
    
    # Generate HTML coverage report
    gcovr -r .. --html --html-details -o coverage/index.html
    gcovr -r .. --xml -o coverage/coverage.xml
    
    echo "✅ Coverage report generated at: build/coverage/index.html"
fi

cd ..

echo ""
echo "========================================"\n
echo "✅ All tests completed successfully!"
echo "========================================"
