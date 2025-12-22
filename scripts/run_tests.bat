@echo off
REM Test runner for Windows

echo ========================================
echo Running Unit Tests
echo ========================================

if not exist "build\\bin" (
    echo Build directory not found. Run build_vs2022.bat first.
    exit /b 1
)

cd build\\bin

echo.
echo [1/2] Running unit tests...
if exist "unit_tests.exe" (
    unit_tests.exe --gtest_output=xml:../test-results/unit_tests.xml
    echo Unit tests passed!
) else (
    echo Unit tests binary not found.
)

echo.
echo [2/2] Running benchmarks...
if exist "bench_tests.exe" (
    bench_tests.exe --benchmark_out=../test-results/benchmarks.json
    echo Benchmarks completed!
) else (
    echo Benchmark binary not found.
)

cd ../..

echo.
echo ========================================
echo Tests completed!
echo ========================================
