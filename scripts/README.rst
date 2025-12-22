Scripts Directory
=================

Automation scripts for building, testing, and documentation.

Scripts
-------

Build Scripts
^^^^^^^^^^^^^
- **build_vs2022.bat**: Windows Visual Studio 2022 build
- **build_xcode.sh**: macOS Xcode project generation
- **build_clion.sh**: Linux CLion build configuration

Test Scripts
^^^^^^^^^^^^
- **run_tests.sh**: Unix automated test runner with coverage
- **run_tests.bat**: Windows automated test runner

Documentation Scripts
^^^^^^^^^^^^^^^^^^^^^
- **generate_docs.sh**: Doxygen API documentation generation

Usage
-----

All scripts should be executed from the project root directory:

```bash
# Build
./scripts/build_xcode.sh        # macOS
./scripts/build_clion.sh        # Linux
scripts\build_vs2022.bat         # Windows

# Test
./scripts/run_tests.sh          # Unix
./scripts/run_tests.sh --coverage  # With coverage
scripts\run_tests.bat            # Windows

# Documentation
./scripts/generate_docs.sh
```
