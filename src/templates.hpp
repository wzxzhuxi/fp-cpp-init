#pragma once

#include <string_view>

namespace fp::templates {

// =============================================================================
// CMakeLists.txt templates
// =============================================================================

constexpr const char* cmake_exe = R"(cmake_minimum_required(VERSION 3.20)

project(
    {{PROJECT_NAME}}
    VERSION 0.1.0
    DESCRIPTION "{{DESCRIPTION}}"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD {{CXX_STD}})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Code coverage option
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

# Cross-platform compile options
if(MSVC)
    add_compile_options(/W4 /utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror=return-type)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
    add_compile_options(-g -O0)
elseif(NOT MSVC)
    add_compile_options(-O3 -DNDEBUG)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Tests
option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
)";

constexpr const char* cmake_lib = R"(cmake_minimum_required(VERSION 3.20)

project(
    {{PROJECT_NAME}}
    VERSION 0.1.0
    DESCRIPTION "{{DESCRIPTION}}"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD {{CXX_STD}})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Code coverage option
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

# Cross-platform compile options
if(MSVC)
    add_compile_options(/W4 /utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror=return-type)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Tests
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)
)";

constexpr const char* cmake_header = R"(cmake_minimum_required(VERSION 3.20)

project(
    {{PROJECT_NAME}}
    VERSION 0.1.0
    DESCRIPTION "{{DESCRIPTION}}"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD {{CXX_STD}})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Code coverage option
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

# Cross-platform compile options
if(MSVC)
    add_compile_options(/W4 /utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror=return-type)
endif()

# Header-only library
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Examples
add_subdirectory(examples)

# Tests
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install
install(DIRECTORY include/ DESTINATION include)
)";

constexpr const char* cmake_examples = R"(add_executable(example example.cpp)
target_link_libraries(example PRIVATE {{PROJECT_NAME}})
)";

constexpr const char* cmake_tests = R"(# Tests using simple assertions
# For a proper test framework, consider Catch2:
# include(FetchContent)
# FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.5.2)
# FetchContent_MakeAvailable(Catch2)

add_executable(tests test_main.cpp)
target_link_libraries(tests PRIVATE {{PROJECT_NAME}})

# CTest integration
add_test(NAME {{PROJECT_NAME}}_tests COMMAND tests)
)";

// =============================================================================
// Result<T> template (Functional Error Handling)
// =============================================================================

constexpr const char* result_hpp = R"(#pragma once

#include <string>
#include <variant>

namespace {{PROJECT_NAME}} {

/**
 * @brief Result<T> - Functional error handling monad
 *
 * Represents either a successful value (Ok) or an error message (Err).
 * Enables explicit error handling without exceptions.
 *
 * Usage:
 *   auto result = parse_int("42");
 *   if (result.is_ok()) {
 *       use(result.value());
 *   } else {
 *       handle_error(result.error());
 *   }
 */
template<typename T>
class Result {
public:
    static auto ok(T value) -> Result { return Result{std::move(value)}; }
    static auto err(std::string error) -> Result { return Result{std::move(error)}; }

    [[nodiscard]] auto is_ok() const -> bool { return std::holds_alternative<T>(data_); }
    [[nodiscard]] auto is_err() const -> bool { return !is_ok(); }

    [[nodiscard]] auto value() const -> const T& { return std::get<T>(data_); }
    [[nodiscard]] auto error() const -> const std::string& { return std::get<std::string>(data_); }

    /**
     * @brief Transform the value if Ok, propagate error if Err
     */
    template<typename F>
    auto map(F&& f) const -> Result<decltype(f(std::declval<T>()))> {
        using U = decltype(f(std::declval<T>()));
        if (is_ok()) {
            return Result<U>::ok(f(value()));
        }
        return Result<U>::err(error());
    }

    /**
     * @brief Chain operations that return Result
     */
    template<typename F>
    auto and_then(F&& f) const -> decltype(f(std::declval<T>())) {
        if (is_ok()) {
            return f(value());
        }
        return decltype(f(std::declval<T>()))::err(error());
    }

private:
    explicit Result(T val) : data_(std::move(val)) {}
    explicit Result(std::string err) : data_(std::move(err)) {}

    std::variant<T, std::string> data_;
};

} // namespace {{PROJECT_NAME}}
)";

// =============================================================================
// Source file templates
// =============================================================================

constexpr const char* main_cpp = R"(#include <iostream>
#include <string_view>

#include "{{PROJECT_NAME}}/result.hpp"

namespace {{PROJECT_NAME}} {

// =============================================================================
// Pure Functions - No side effects, same input always produces same output
// =============================================================================

/**
 * @brief Pure function: creates a greeting message
 */
auto greet(std::string_view name) -> std::string {
    return std::string("Hello, ") + std::string(name) + "!";
}

/**
 * @brief Pure function: adds two integers
 */
auto add(int a, int b) -> int {
    return a + b;
}

/**
 * @brief Pure function with Result: parses string to integer
 * Returns Result<int> instead of throwing exceptions
 */
auto parse_int(std::string_view input) -> Result<int> {
    try {
        size_t pos = 0;
        int result = std::stoi(std::string(input), &pos);
        if (pos != input.size()) {
            return Result<int>::err("Invalid integer: trailing characters in '" + std::string(input) + "'");
        }
        return Result<int>::ok(result);
    } catch (const std::exception&) {
        return Result<int>::err("Invalid integer: '" + std::string(input) + "'");
    }
}

} // namespace {{PROJECT_NAME}}

// =============================================================================
// Main - Side Effect Boundary (all IO happens here)
// =============================================================================

int main() {
    // Call pure functions
    auto message = {{PROJECT_NAME}}::greet("{{PROJECT_NAME}}");
    std::cout << message << std::endl;

    // Demonstrate pure arithmetic
    std::cout << "2 + 3 = " << {{PROJECT_NAME}}::add(2, 3) << std::endl;

    // Use Result for error handling (no exceptions)
    auto result = {{PROJECT_NAME}}::parse_int("42");
    if (result.is_ok()) {
        std::cout << "Parsed: " << result.value() << std::endl;
    }

    // Demonstrate error case
    auto bad_result = {{PROJECT_NAME}}::parse_int("not_a_number");
    if (bad_result.is_err()) {
        std::cout << "Error: " << bad_result.error() << std::endl;
    }

    return 0;
}
)";

constexpr const char* lib_hpp = R"(#pragma once

#include <string>
#include <string_view>

#include "{{PROJECT_NAME}}/result.hpp"

namespace {{PROJECT_NAME}} {

// =============================================================================
// Pure Functions - No side effects, same input always produces same output
// =============================================================================

/**
 * @brief Pure function: creates a greeting message
 */
auto greet(std::string_view name) -> std::string;

/**
 * @brief Pure function: adds two integers
 */
auto add(int a, int b) -> int;

/**
 * @brief Pure function with Result: parses string to integer
 * Returns Result<int> instead of throwing exceptions
 */
auto parse_int(std::string_view input) -> Result<int>;

} // namespace {{PROJECT_NAME}}
)";

constexpr const char* lib_cpp = R"(#include "{{PROJECT_NAME}}/{{PROJECT_NAME}}.hpp"

namespace {{PROJECT_NAME}} {

auto greet(std::string_view name) -> std::string {
    return std::string("Hello, ") + std::string(name) + "!";
}

auto add(int a, int b) -> int {
    return a + b;
}

auto parse_int(std::string_view input) -> Result<int> {
    try {
        size_t pos = 0;
        int result = std::stoi(std::string(input), &pos);
        if (pos != input.size()) {
            return Result<int>::err("Invalid integer: trailing characters in '" + std::string(input) + "'");
        }
        return Result<int>::ok(result);
    } catch (const std::exception&) {
        return Result<int>::err("Invalid integer: '" + std::string(input) + "'");
    }
}

} // namespace {{PROJECT_NAME}}
)";

constexpr const char* header_only_hpp = R"(#pragma once

#include <string>
#include <string_view>
#include <variant>

namespace {{PROJECT_NAME}} {

// =============================================================================
// Result<T> - Functional Error Handling Monad
// =============================================================================

/**
 * @brief Result<T> - Functional error handling monad
 *
 * Represents either a successful value (Ok) or an error message (Err).
 * Enables explicit error handling without exceptions.
 */
template<typename T>
class Result {
public:
    static auto ok(T value) -> Result { return Result{std::move(value)}; }
    static auto err(std::string error) -> Result { return Result{std::move(error)}; }

    [[nodiscard]] auto is_ok() const -> bool { return std::holds_alternative<T>(data_); }
    [[nodiscard]] auto is_err() const -> bool { return !is_ok(); }

    [[nodiscard]] auto value() const -> const T& { return std::get<T>(data_); }
    [[nodiscard]] auto error() const -> const std::string& { return std::get<std::string>(data_); }

    template<typename F>
    auto map(F&& f) const -> Result<decltype(f(std::declval<T>()))> {
        using U = decltype(f(std::declval<T>()));
        if (is_ok()) {
            return Result<U>::ok(f(value()));
        }
        return Result<U>::err(error());
    }

    template<typename F>
    auto and_then(F&& f) const -> decltype(f(std::declval<T>())) {
        if (is_ok()) {
            return f(value());
        }
        return decltype(f(std::declval<T>()))::err(error());
    }

private:
    explicit Result(T val) : data_(std::move(val)) {}
    explicit Result(std::string err) : data_(std::move(err)) {}

    std::variant<T, std::string> data_;
};

// =============================================================================
// Pure Functions - No side effects, same input always produces same output
// =============================================================================

/**
 * @brief Pure function: creates a greeting message
 */
inline auto greet(std::string_view name) -> std::string {
    return std::string("Hello, ") + std::string(name) + "!";
}

/**
 * @brief Pure function: adds two integers
 */
inline auto add(int a, int b) -> int {
    return a + b;
}

/**
 * @brief Pure function with Result: parses string to integer
 * Returns Result<int> instead of throwing exceptions
 */
inline auto parse_int(std::string_view input) -> Result<int> {
    try {
        size_t pos = 0;
        int result = std::stoi(std::string(input), &pos);
        if (pos != input.size()) {
            return Result<int>::err("Invalid integer: trailing characters in '" + std::string(input) + "'");
        }
        return Result<int>::ok(result);
    } catch (const std::exception&) {
        return Result<int>::err("Invalid integer: '" + std::string(input) + "'");
    }
}

} // namespace {{PROJECT_NAME}}
)";

constexpr const char* example_cpp = R"(#include <iostream>

#include "{{PROJECT_NAME}}/{{PROJECT_NAME}}.hpp"

// =============================================================================
// Example - Side Effect Boundary (all IO happens here)
// =============================================================================

int main() {
    // Call pure functions
    std::cout << {{PROJECT_NAME}}::greet("World") << std::endl;

    // Demonstrate pure arithmetic
    std::cout << "2 + 3 = " << {{PROJECT_NAME}}::add(2, 3) << std::endl;

    // Use Result for error handling
    auto result = {{PROJECT_NAME}}::parse_int("42");
    if (result.is_ok()) {
        std::cout << "Parsed: " << result.value() << std::endl;
    }

    // Demonstrate error handling
    auto bad_result = {{PROJECT_NAME}}::parse_int("not_a_number");
    if (bad_result.is_err()) {
        std::cout << "Error: " << bad_result.error() << std::endl;
    }

    return 0;
}
)";

constexpr const char* test_main_cpp = R"(#include <cassert>
#include <iostream>

#include "{{PROJECT_NAME}}/{{PROJECT_NAME}}.hpp"

// =============================================================================
// Tests for Pure Functions
// =============================================================================

void test_greet() {
    assert({{PROJECT_NAME}}::greet("World") == "Hello, World!");
    assert({{PROJECT_NAME}}::greet("") == "Hello, !");
    std::cout << "[PASS] test_greet" << std::endl;
}

void test_add() {
    assert({{PROJECT_NAME}}::add(2, 3) == 5);
    assert({{PROJECT_NAME}}::add(-1, 1) == 0);
    assert({{PROJECT_NAME}}::add(0, 0) == 0);
    std::cout << "[PASS] test_add" << std::endl;
}

void test_parse_int_success() {
    auto result = {{PROJECT_NAME}}::parse_int("42");
    assert(result.is_ok());
    assert(result.value() == 42);

    auto negative = {{PROJECT_NAME}}::parse_int("-123");
    assert(negative.is_ok());
    assert(negative.value() == -123);

    std::cout << "[PASS] test_parse_int_success" << std::endl;
}

void test_parse_int_failure() {
    auto result = {{PROJECT_NAME}}::parse_int("abc");
    assert(result.is_err());

    auto empty = {{PROJECT_NAME}}::parse_int("");
    assert(empty.is_err());

    auto trailing = {{PROJECT_NAME}}::parse_int("42abc");
    assert(trailing.is_err());

    std::cout << "[PASS] test_parse_int_failure" << std::endl;
}

void test_result_map() {
    auto result = {{PROJECT_NAME}}::parse_int("10");
    auto doubled = result.map([](int x) { return x * 2; });
    assert(doubled.is_ok());
    assert(doubled.value() == 20);

    auto err = {{PROJECT_NAME}}::parse_int("invalid");
    auto doubled_err = err.map([](int x) { return x * 2; });
    assert(doubled_err.is_err());

    std::cout << "[PASS] test_result_map" << std::endl;
}

int main() {
    std::cout << "Running tests for {{PROJECT_NAME}}..." << std::endl;
    std::cout << std::endl;

    test_greet();
    test_add();
    test_parse_int_success();
    test_parse_int_failure();
    test_result_map();

    std::cout << std::endl;
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
)";

// =============================================================================
// Config file templates
// =============================================================================

constexpr const char* gitignore = R"(# Build directories
build/
cmake-build-*/
out/

# IDE
.idea/
.vscode/
*.swp
*.swo
*~

# Clangd cache
.cache/

# Compile commands (generated)
compile_commands.json

# Object files
*.o
*.obj

# Libraries
*.a
*.lib
*.so
*.dylib

# Executables
*.exe
*.out

# macOS
.DS_Store

# Debug
*.dSYM/
)";

constexpr const char* clang_format = R"(BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 100
AllowShortFunctionsOnASingleLine: Inline
AllowShortLambdasOnASingleLine: All
BinPackArguments: false
BinPackParameters: false
BreakBeforeBraces: Attach
PointerAlignment: Left
)";

constexpr const char* clang_tidy = R"(Checks: >
    bugprone-*,
    cppcoreguidelines-*,
    modernize-*,
    performance-*,
    readability-*,
    -modernize-use-trailing-return-type,
    -readability-identifier-length,
    -cppcoreguidelines-avoid-magic-numbers,
    -readability-magic-numbers

WarningsAsErrors: ''
HeaderFilterRegex: '.*'
)";

// =============================================================================
// License templates
// =============================================================================

constexpr const char* license_mit = R"(MIT License

Copyright (c) {{YEAR}} {{AUTHOR}}

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
)";

constexpr const char* license_apache2 = R"(                              Apache License
                        Version 2.0, January 2004
                     http://www.apache.org/licenses/

Copyright {{YEAR}} {{AUTHOR}}

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
)";

constexpr const char* license_gpl3 = R"({{PROJECT_NAME}} - {{DESCRIPTION}}
Copyright (C) {{YEAR}} {{AUTHOR}}

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
)";

constexpr const char* license_bsd3 = R"(BSD 3-Clause License

Copyright (c) {{YEAR}}, {{AUTHOR}}
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
)";

// =============================================================================
// README template
// =============================================================================

constexpr const char* readme = R"(# {{PROJECT_NAME}}

[![CI](https://github.com/YOUR_USERNAME/{{PROJECT_NAME}}/actions/workflows/ci.yml/badge.svg)](https://github.com/YOUR_USERNAME/{{PROJECT_NAME}}/actions/workflows/ci.yml)

{{DESCRIPTION}}

## Build

```bash
cmake -B build -G Ninja
cmake --build build
```

## Usage

```bash
./build/{{PROJECT_NAME}}
```

## License

{{LICENSE_NAME}}
)";

constexpr const char* readme_lib = R"(# {{PROJECT_NAME}}

[![CI](https://github.com/YOUR_USERNAME/{{PROJECT_NAME}}/actions/workflows/ci.yml/badge.svg)](https://github.com/YOUR_USERNAME/{{PROJECT_NAME}}/actions/workflows/ci.yml)

{{DESCRIPTION}}

## Build

```bash
cmake -B build -G Ninja
cmake --build build
```

## Usage

```cpp
#include "{{PROJECT_NAME}}/{{PROJECT_NAME}}.hpp"

int main() {
    auto msg = {{PROJECT_NAME}}::hello();
    return 0;
}
```

## Install

```bash
cmake --install build --prefix /usr/local
```

## License

{{LICENSE_NAME}}
)";

// =============================================================================
// GitHub Actions templates
// =============================================================================

constexpr const char* github_ci = R"(name: CI

on:
  push:
    branches: [main]
  pull_request:
    branches: [main]

jobs:
  format:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Check clang-format
        run: find src include -name '*.cpp' -o -name '*.hpp' | xargs clang-format --dry-run --Werror 2>/dev/null || true

  build:
    strategy:
      fail-fast: false
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]

    runs-on: ${{ matrix.os }}

    steps:
      - uses: actions/checkout@v4

      - name: Configure CMake
        run: cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON

      - name: Build
        run: cmake --build build --config Release

      - name: Run tests
        run: ctest --test-dir build --output-on-failure --config Release

  coverage:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4

      - name: Install lcov
        run: sudo apt-get install -y lcov

      - name: Configure with coverage
        run: cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCODE_COVERAGE=ON -DBUILD_TESTING=ON

      - name: Build
        run: cmake --build build

      - name: Run tests
        run: ctest --test-dir build --output-on-failure

      - name: Collect coverage
        run: |
          lcov --capture --directory build --output-file coverage.info
          lcov --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage.info

      - name: Upload to Codecov
        uses: codecov/codecov-action@v4
        with:
          files: coverage.info
          fail_ci_if_error: false
)";

constexpr const char* github_release = R"(name: Release

on:
  push:
    tags:
      - 'v*'

permissions:
  contents: write

jobs:
  build:
    strategy:
      matrix:
        include:
          - os: ubuntu-latest
            artifact: {{PROJECT_NAME}}-linux-x86_64
            binary: build/{{PROJECT_NAME}}
          - os: macos-latest
            artifact: {{PROJECT_NAME}}-macos-x86_64
            binary: build/{{PROJECT_NAME}}
          - os: windows-latest
            artifact: {{PROJECT_NAME}}-windows-x86_64.exe
            binary: build/Release/{{PROJECT_NAME}}.exe

    runs-on: ${{ matrix.os }}

    steps:
      - uses: actions/checkout@v4

      - name: Configure CMake
        run: cmake -B build -DCMAKE_BUILD_TYPE=Release

      - name: Build
        run: cmake --build build --config Release

      - name: Rename Binary (Unix)
        if: runner.os != 'Windows'
        run: mv ${{ matrix.binary }} ${{ matrix.artifact }}

      - name: Rename Binary (Windows)
        if: runner.os == 'Windows'
        run: move ${{ matrix.binary }} ${{ matrix.artifact }}

      - name: Upload Artifact
        uses: actions/upload-artifact@v4
        with:
          name: ${{ matrix.artifact }}
          path: ${{ matrix.artifact }}

  release:
    needs: build
    runs-on: ubuntu-latest

    steps:
      - name: Download All Artifacts
        uses: actions/download-artifact@v4
        with:
          path: artifacts

      - name: Create Release
        uses: softprops/action-gh-release@v2
        with:
          files: artifacts/*/*
        env:
          GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
)";

} // namespace fp::templates
