
# Ariadne Paradigm

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Unix Status](https://github.com/ariadne-cps/paradigm/workflows/Unix/badge.svg)](https://github.com/ariadne-cps/paradigm/actions/workflows/unix.yml)
[![Windows Status](https://github.com/ariadne-cps/paradigm/workflows/Windows/badge.svg)](https://github.com/ariadne-cps/paradigm/actions/workflows/win.yml) [![Coverage Status](https://github.com/ariadne-cps/paradigm/workflows/Coverage/badge.svg)](https://github.com/ariadne-cps/paradigm/actions/workflows/coverage.yml) [![codecov](https://codecov.io/gh/ariadne-cps/paradigm/branch/main/graph/badge.svg)](https://codecov.io/gh/ariadne-cps/paradigm)

Paradigm provides the low-level computational paradigms and logical types used by Ariadne.

Its only Ariadne dependency is [ariadne-cps/utility](https://github.com/ariadne-cps/utility). Build configuration is shared through [ariadne-cps/configuration](https://github.com/ariadne-cps/configuration).

## Build

Clone the repository together with its Git submodules:

```bash
git clone --recurse-submodules https://github.com/ariadne-cps/paradigm.git
cd paradigm
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
ctest --output-on-failure
```

A C++20 compiler and CMake are required.

## Coverage

Configure a separate Debug build with coverage enabled:

```bash
mkdir build-coverage
cd build-coverage
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=ON
cmake --build . --parallel --target coverage
```

On Ubuntu coverage is generated with GCC/lcov. On macOS it is generated with AppleClang/LLVM coverage tools.
