# Contributing to Conch Cross-Platform

Thank you for considering contributing to this project! This document provides guidelines for contributions.

## Code of Conduct

- Be respectful and inclusive
- Provide constructive feedback
- Focus on what is best for the community

## How to Contribute

### Reporting Bugs

1. Check if the bug has already been reported in [Issues](../../issues)
2. If not, create a new issue with:
   - Clear title and description
   - Steps to reproduce
   - Expected vs actual behavior
   - Environment details (OS, compiler, etc.)

### Suggesting Features

1. Open an issue with the `enhancement` label
2. Describe the feature and its use case
3. Explain why it would be valuable

### Pull Requests

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/amazing-feature`
3. **Make your changes**:
   - Follow the coding style (see below)
   - Add tests for new functionality
   - Update documentation as needed
4. **Commit your changes**: `git commit -m 'Add amazing feature'`
5. **Push to the branch**: `git push origin feature/amazing-feature`
6. **Open a Pull Request**

## Development Setup

### Prerequisites

- CMake 3.25+
- Conan 2.x
- C++26 compatible compiler:
  - GCC 14+
  - Clang 18+
  - MSVC 17.9+

### Building

```bash
# Clone the repository
git clone https://github.com/yourusername/conch-cpp.git
cd conch-cpp

# Build
./build.sh              # Linux/macOS
scripts\build_vs2022.bat  # Windows
```

### Running Tests

```bash
./scripts/run_tests.sh          # Linux/macOS
scripts\run_tests.bat            # Windows
```

## Coding Style

### C++ Guidelines

- Use C++26 features where appropriate
- Follow [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- Use `clang-format` with the provided `.clang-format` file
- Prefer `const` and `constexpr` where possible
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- Avoid raw `new`/`delete`

### Naming Conventions

- **Classes/Structs**: `PascalCase`
- **Functions/Methods**: `snake_case`
- **Variables**: `snake_case`
- **Constants**: `UPPER_SNAKE_CASE`
- **Namespaces**: `lowercase`

### Documentation

- Use Doxygen-style comments for public APIs:

```cpp
/**
 * @brief Brief description
 * @param param_name Parameter description
 * @return Return value description
 */
int my_function(int param_name);
```

## Testing

- Write unit tests for all new functionality
- Ensure all tests pass before submitting PR
- Aim for >80% code coverage
- Use Google Test framework

## Commit Messages

Follow [Conventional Commits](https://www.conventionalcommits.org/):

```
feat: Add new trading algorithm
fix: Resolve memory leak in network module
docs: Update API documentation
test: Add benchmark tests for quant_core
refactor: Simplify media_core signaling logic
```

## Review Process

1. All PRs require at least one approval
2. CI/CD must pass (builds, tests, linting)
3. Code coverage should not decrease
4. Documentation must be updated

## License

By contributing, you agree that your contributions will be licensed under the project's license.

## Questions?

Feel free to open an issue or contact the maintainers.

Thank you for contributing! 🎉
