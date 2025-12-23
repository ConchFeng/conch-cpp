=========================
Conch Cross-Platform C++
=========================

.. image:: https://img.shields.io/badge/C%2B%2B-23-blue.svg
   :alt: C++23

.. image:: https://img.shields.io/badge/License-MIT-green.svg
   :alt: License

.. image:: https://img.shields.io/badge/CMake-3.25%2B-blue.svg
   :alt: CMake

.. image:: https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Linux%20%7C%20iOS%20%7C%20Android-lightgrey
   :alt: Platforms

**A production-ready, batteries-included C++ framework for building everything from games to financial apps — in minutes, not months.**

🚀 **Build faster. Ship sooner. Scale forever.**

💡 Tagline
==========

*"From Hello World to Production in One Framework"*

✨ What Is This?
================

**Conch** is a modern, enterprise-grade C++ cross-platform framework that gets you from idea to deployed application at lightning speed. Whether you're building:

- 🎮 **Games** (Snake, interactive apps)
- 💰 **Trading Systems** (HFT engines, stock simulators)
- 💬 **Real-Time Communication** (Chat servers, meeting gateways)  
- 🌐 **Network Services** (TCP/UDP servers, WebSocket)
- 📊 **Data Processing** (Analytics, monitoring)

**Conch has you covered** with a battle-tested architecture, modern tooling, and zero boilerplate.

🎯 Why Choose Conch?
====================

❌ **Before Conch:**
   - Weeks spent on project setup
   - CMake configuration hell
   - Dependency management nightmares
   - IDE incompatibilities
   - No CI/CD templates
   
✅ **With Conch:**
   - **5-Second Setup**: Clone, ``./build.sh``, done.
   - **Any IDE**: VS2022, Xcode, CLion, VSCode — all pre-configured
   - **Modern Dependencies**: Conan 2.x handles everything automatically
   - **Production-Ready CI/CD**: Jenkins, GitHub Actions, GitLab CI templates included
   - **Docker Ready**: Multi-stage builds, docker-compose orchestration

⚡ Quick Start
==============

**Get running in 3 commands:**

.. code-block:: bash

   git clone https://github.com/ConchFeng/conch-cpp.git
   cd conch-cpp
   ./build.sh

**That's it.** You now have:

- ✅ HFT trading engine
- ✅ RTC meeting gateway  
- ✅ Stock trading simulator
- ✅ Multi-client chat system
- ✅ Snake game (CLI + Qt GUI)
- ✅ Unit tests + benchmarks

All built, all tested, all running.

📦 What's Included?
===================

Pre-Built Applications
-----------------------

=========== ================================= ============== ===================
Category    Application                       Technology     Platforms
=========== ================================= ============== ===================
**Games**   Snake Game (CLI)                  Terminal UI    macOS, Linux
**Games**   Snake Game (Qt)                   Qt6 Widgets    Win, macOS, Linux
**Finance** Stock Trading Simulator           Portfolio Mgmt Win, macOS, Linux
**Network** Chat Server                       libuv TCP      Win, macOS, Linux
**Network** Chat Client                       Async I/O      Win, macOS, Linux
**HFT**     Trading Engine                    High-Perf      Win, macOS, Linux
**RTC**     Meeting Gateway                   WebRTC Ready   Win, macOS, Linux
**Desktop** Pro Desktop App                   Qt6 Cross-Plat Win, macOS, Linux
**Mobile**  iOS Native App                    UIKit          iOS only
**Mobile**  Android Native App                JNI            Android only
=========== ================================= ============== ===================

.. note::
   **Snake CLI** requires Unix terminal APIs and is not available on Windows. 
   Windows users can use **Snake Qt** instead for a graphical experience.

Foundation Libraries
--------------------

- **foundation**: Logging, threading, utilities
- **network**: High-performance networking (libuv)
- **quant_core**: Quantitative finance models
- **media_core**: RTC/Meeting infrastructure
- **ui_kit**: Qt6 UI components

Development Tools
-----------------

✅ **Multi-IDE Support**
   - Visual Studio 2022 (Windows)
   - Xcode (macOS)
   - CLion (Linux)
   - VSCode (All platforms)

✅ **CI/CD Pipelines**
   - Jenkins (Declarative pipeline)
   - GitHub Actions (Matrix builds)
   - GitLab CI (Multi-stage)

✅ **Containerization**
   - Dockerfile (Multi-stage production)
   - docker-compose (Service orchestration)

✅ **Documentation**
   - Doxygen (API docs with diagrams)
   - Auto-generation scripts

✅ **Testing**
   - GTest (Unit tests)
   - Google Benchmark (Performance)
   - Coverage reports (gcovr)

🏗️ Architecture
================

Clean, modular, scalable:

.. code-block:: text

   conch-cpp/
   ├── src/
   │   ├── libs/              # Reusable libraries
   │   ├── servers/           # Backend services
   │   └── apps/              # End-user applications
   ├── tests/                 # Unit tests + benchmarks
   ├── scripts/               # Build automation
   ├── .github/workflows/     # CI/CD pipelines
   └── docker/                # Containerization

🚀 Usage Examples
=================

**Run a Chat Server:**

.. code-block:: bash

   ./run.sh chat_server
   # Server listening on port 8888

**Connect Chat Clients:**

.. code-block:: bash

   ./run.sh chat_client
   # Start typing to chat!

**Trade Stocks:**

.. code-block:: bash

   ./run.sh stock_trader
   # $100K virtual portfolio, 8 US stocks

**Play Snake:**

.. code-block:: bash

   ./run.sh snake_cli      # Terminal version
   ./run.sh snake_qt       # Qt GUI version

🛠️ Build Your Own App
=======================

**Add a new application in 3 steps:**

1. Create directory: ``src/apps/my_app/``
2. Add ``CMakeLists.txt`` (5 lines)
3. Run ``./build.sh``

**Example CMakeLists.txt:**

.. code-block:: cmake

   add_executable(my_app main.cpp)
   target_link_libraries(my_app 
       PRIVATE foundation spdlog::spdlog)

Done. Your app is integrated into the framework.

🌟 Technology Stack
===================

========================  =============
Component                 Technology
========================  =============
Language                  **C++23**
Build System              **CMake 3.25+**
Package Manager           **Conan 2.x**
Desktop UI                **Qt6**
Mobile (iOS)              Native (UIKit)
Mobile (Android)          Native (JNI)
Logging                   **spdlog**
Networking                **libuv**
Testing                   **GTest**
Benchmarking              **Google Benchmark**
Documentation             **Doxygen**
CI/CD                     Jenkins, GitHub Actions
Containerization          **Docker**
========================  =============

📊 Production-Ready Features
=============================

✅ **Cross-Platform**
   - Windows, macOS, Linux (desktop)
   - iOS, Android (mobile native)

✅ **Enterprise Build System**
   - Conan 2.x dependency management
   - CMake presets for all environments
   - One-command builds

✅ **Multi-IDE Support**
   - Pre-configured for 4 major IDEs
   - Just open and code

✅ **Automated Testing**
   - Unit tests with GTest
   - Benchmarks with Google Benchmark
   - Coverage reports with one command

✅ **CI/CD Ready**
   - GitHub Actions workflows
   - Jenkins declarative pipelines
   - GitLab CI configurations

✅ **Docker Deployment**
   - Multi-stage builds (small images)
   - docker-compose orchestration
   - Production-ready containers

✅ **API Documentation**
   - Doxygen auto-generation
   - Call graphs, UML diagrams
   - One-command: ``./scripts/generate_docs.sh``

🎓 Perfect For
==============

✅ **Startups**: Ship MVPs in days, not months  
✅ **Enterprises**: Production-ready architecture  
✅ **Indie Developers**: Focus on features, not plumbing  
✅ **Students**: Learn modern C++ best practices  
✅ **Game Devs**: Cross-platform game framework  
✅ **FinTech**: HFT/trading infrastructure built-in  

📚 Documentation
================

- **Quick Start Guide**: `See above ↑`
- **API Documentation**: Run ``./scripts/generate_docs.sh``
- **Contributing**: See `CONTRIBUTING.md <CONTRIBUTING.md>`_
- **Build Scripts**: See `scripts/ <scripts/>`_

🤝 Contributing
===============

We love contributions! See `CONTRIBUTING.md <CONTRIBUTING.md>`_ for:

- Code style guidelines
- Pull request process
- Development setup
- Testing requirements

📄 License
==========

MIT License - Use it, modify it, ship it. See `LICENSE <LICENSE>`_.

🌟 Show Your Support
====================

If you find Conch useful, give us a ⭐ on GitHub!

💬 Community
============

- **Issues**: `GitHub Issues <https://github.com/ConchFeng/conch-cpp/issues>`_
- **Discussions**: `GitHub Discussions <https://github.com/ConchFeng/conch-cpp/discussions>`_

---

**Built with ❤️ for the C++ community**

*Don't just code. Ship.*
