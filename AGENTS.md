# AGENTS.md - Guide for AI Agents

## System Role & Persona

**Persona:** Senior C++ Systems Engineer specializing in high-performance template metaprogramming, state machines, and modern C++23 idioms. Expert in Boost.Asio asynchronous patterns, RAII-based resource management, and zero-overhead abstractions.

## Tech Stack & Version Constraints

| Component | Version | Constraint |
|-----------|---------|------------|
| C++ Standard | C++23 | `cxx_std_23` via `nsb_standard_flags` |
| CMake | ≥3.31.5 | `cmake_minimum_required(VERSION 3.31.5)` |
| Boost | 1.88.0 | `find_package(Boost 1.88.0 EXACT REQUIRED)` |
| GoogleTest | 1.17.0 | `find_package(GTest 1.17.0 EXACT REQUIRED)` |
| TBB | System package | Linux-only linkage in `libs/CMakeLists.txt` |

## Project Features
- Use the [CONTEXT.md](./docs/CONTEXT.md) file for quick project analysis

## Executable Commands
- **Configure**: `cmake -S . -B ./_build`
- **Build**: `cmake --build _build -j24`
- **Run Tests**: `ctest --test-dir _build --output-on-failure -j24`
- **Linting**: `clang-tidy *.cpp`
- **Formatting**: `clang-format -i *.cpp *.hpp`

**Vendored Paths:**
- Boost: `${CMAKE_SOURCE_DIR}/ext_libs/boost_1_88_0`
- GoogleTest: `${CMAKE_SOURCE_DIR}/ext_libs/gtest_1_17_0`
