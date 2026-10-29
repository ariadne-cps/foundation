
# Ariadne Foundation

[![macOS Status](https://github.com/ariadne-cps/foundation/actions/workflows/macos.yml/badge.svg)](https://github.com/ariadne-cps/foundation/actions/workflows/macos.yml)
[![Ubuntu Status](https://github.com/ariadne-cps/foundation/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/ariadne-cps/foundation/actions/workflows/ubuntu.yml)
[![Windows Status](https://github.com/ariadne-cps/foundation/actions/workflows/win.yml/badge.svg)](https://github.com/ariadne-cps/foundation/actions/workflows/win.yml)
[![Sanitizers Status](https://github.com/ariadne-cps/foundation/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/ariadne-cps/foundation/actions/workflows/sanitizers.yml)
[![Coverage Status](https://github.com/ariadne-cps/foundation/actions/workflows/coverage.yml/badge.svg)](https://github.com/ariadne-cps/foundation/actions/workflows/coverage.yml)
[![codecov](https://codecov.io/gh/ariadne-cps/foundation/branch/main/graph/badge.svg)](https://codecov.io/gh/ariadne-cps/foundation)

Foundation provides the low-level computational paradigms and logical types used by Ariadne.

Its only Ariadne dependency is [ariadne-cps/utility](https://github.com/ariadne-cps/utility). Build configuration is shared through [ariadne-cps/configuration](https://github.com/ariadne-cps/configuration).

## Build

Clone the repository together with its Git submodules:

```bash
git clone --recurse-submodules https://github.com/ariadne-cps/foundation.git
cd foundation
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel --target ariadne-foundation bindings
cmake --install build-release --prefix install
```

A C++20 compiler, CMake and Python 3.11+ with development headers are required
for building the optional Python bindings.

## Tests

Build and run the C++ and Python tests independently in Debug mode.
Install `pytest` for the chosen Python interpreter first.

```bash
python -m pip install pytest
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug -DPYTHON_ENFORCE_TESTS=ON
cmake --build build-debug --parallel --target tests
ctest --test-dir build-debug -LE python --output-on-failure
ctest --test-dir build-debug -L python --output-on-failure
```

## Sanitizers

The Sanitizers workflow runs two independent Debug jobs on Ubuntu with Clang:
ThreadSanitizer for data races, and AddressSanitizer + UndefinedBehaviorSanitizer
for memory errors and undefined behavior. Python bindings are disabled in these jobs.

## Coverage

Configure a separate Debug build with coverage enabled:

```bash
mkdir build-coverage
cd build-coverage
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=ON
cmake --build . --parallel --target coverage
```

On Ubuntu coverage is generated with GCC/lcov. On macOS it is generated with AppleClang/LLVM coverage tools.
