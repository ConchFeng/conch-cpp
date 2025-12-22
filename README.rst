=========================
Conch Cross-Platform C++
=========================

.. image:: https://img.shields.io/badge/C%2B%2B-26-blue.svg
   :alt: C++26

.. image:: https://img.shields.io/badge/License-MIT-green.svg
   :alt: License

.. image:: https://img.shields.io/badge/CMake-3.25%2B-blue.svg
   :alt: CMake Version

.. image:: https://github.com/yourusername/conch-cpp/workflows/CI%2FCD%20Pipeline/badge.svg
   :alt: CI/CD Status

A **world-class**, modern C++ cross-platform framework for **High-Frequency Trading (HFT)** and **Real-Time Communication (RTC/Meeting)** applications.

✨ Features
===========

- 🚀 **Modern C++26** with cutting-edge features
- 🌍 **True Cross-Platform**: Windows, macOS, Linux, iOS, Android
- 🏗️ **Modular Architecture**: Clean separation of concerns
- 🧪 **Comprehensive Testing**: Unit tests + benchmarks
- 📦 **Conan 2.x**: Modern dependency management
- 🐳 **Docker Ready**: Multi-stage production builds
- ⚙️ **CI/CD**: Jenkins, GitHub Actions, GitLab CI
- 📚 **Auto Documentation**: Doxygen API docs

🏗️ Architecture
================

::

    src/
    ├── libs/              # Reusable Foundation Libraries
    │   ├── foundation/    # Core utilities, logging, threading
    │   ├── network/       # High-performance networking (libuv)
    │   ├── quant_core/    # Quantitative finance models
    │   ├── media_core/    # RTC/Meeting core logic
    │   └── ui_kit/        # Qt6 UI components (desktop)
    ├── servers/           # Backend Services
    │   ├── trading_engine/  # HFT matching engine
    │   └── meeting_gateway/ # RTC signaling server
    └── apps/              # End-User Applications
        ├── desktop_pro/     # Qt-based desktop app
        ├── ios_native/      # Native iOS (UIKit/SwiftUI)
        └── android_native/  # Native Android (JNI)

🚀 Quick Start
==============

Prerequisites
-------------

- **CMake**: 3.25+
- **Conan**: 2.x
- **Compiler**:
  - GCC 14+ (Linux)
  - Clang 18+ (macOS)
  - MSVC 17.9+ / VS2022 (Windows)

Build
-----

**Linux/macOS:**

.. code-block:: bash

    ./build.sh

**Windows:**

.. code-block:: bat

    scripts\build_vs2022.bat

Run
---

.. code-block:: bash

    ./run.sh trading_engine
    ./run.sh meeting_gateway

🛠️ IDE Support
===============

Visual Studio 2022 (Windows)
-----------------------------

.. code-block:: bat

    scripts\build_vs2022.bat
    start build_vs2022\ConchCrossPlatform.sln

Xcode (macOS)
-------------

.. code-block:: bash

    ./scripts/build_xcode.sh
    open build_xcode/ConchCrossPlatform.xcodeproj

CLion (Linux)
-------------

.. code-block:: bash

    ./scripts/build_clion.sh
    # Open project root in CLion

Visual Studio Code (All Platforms)
-----------------------------------

1. Install recommended extensions (prompted on first open)
2. Press ``Ctrl+Shift+B`` (Build)
3. Press ``F5`` (Debug)

Full configuration in ``.vscode/`` directory.

🐳 Docker
=========

Build Image
-----------

.. code-block:: bash

    docker build -t conch-cpp:latest .

Run Services
------------

.. code-block:: bash

    docker-compose up -d

Services:
- ``trading_engine``: http://localhost:8080
- ``meeting_gateway``: http://localhost:8081

⚙️ CI/CD
========

GitHub Actions
--------------

Automatic triggers on push to ``main``/``develop`` branches.

Features:
- Cross-platform matrix builds (Linux, macOS, Windows)
- Automated testing + coverage
- Docker image building
- Artifact uploads

Jenkins
-------

See ``Jenkinsfile`` for declarative pipeline.

Stages:
1. Checkout
2. Build (parallel: Linux + Docker)
3. Test (unit + coverage)
4. Generate docs
5. Deploy (staging/production)

GitLab CI
---------

See ``.gitlab-ci.yml`` for pipeline configuration.

🧪 Testing
==========

Run Tests
---------

.. code-block:: bash

    # Unix
    ./scripts/run_tests.sh
    
    # With coverage
    ./scripts/run_tests.sh --coverage
    
    # Windows
    scripts\run_tests.bat

Coverage Report
---------------

After running with ``--coverage``, open:

.. code-block:: bash

    open build/coverage/index.html  # macOS
    xdg-open build/coverage/index.html  # Linux

📚 Documentation
================

Generate API Docs
-----------------

.. code-block:: bash

    ./scripts/generate_docs.sh
    open docs/html/index.html

Requires **Doxygen** and **Graphviz**.

🤝 Contributing
===============

We welcome contributions! Please see `CONTRIBUTING.md <CONTRIBUTING.md>`_ for guidelines.

Quick checklist:
- Fork the repository
- Create a feature branch
- Write tests for new code
- Ensure all tests pass
- Update documentation
- Submit a pull request

📄 License
==========

This project is licensed under the MIT License - see the LICENSE file for details.

🌟 Technology Stack
===================

========================  =============
Component                 Technology
========================  =============
Language                  C++26
Build System              CMake 3.25+
Package Manager           Conan 2.x
Desktop UI                Qt6
Mobile iOS                Native (UIKit)
Mobile Android            Native (JNI)
Logging                   spdlog
Networking                libuv
Testing                   GTest
Benchmarking              Google Benchmark
Documentation             Doxygen
CI/CD                     Jenkins, GitHub Actions
Containerization          Docker
========================  =============

📞 Contact
==========

- **Issues**: `GitHub Issues <https://github.com/yourusername/conch-cpp/issues>`_
- **Discussions**: `GitHub Discussions <https://github.com/yourusername/conch-cpp/discussions>`_

---

Made with ❤️ by the Conch Team
