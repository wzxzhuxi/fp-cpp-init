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

add_compile_options(-Wall -Wextra -Wpedantic -Werror=return-type)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0)
else()
    add_compile_options(-O3 -DNDEBUG)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Tests (optional)
# add_subdirectory(tests)
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

add_compile_options(-Wall -Wextra -Wpedantic -Werror=return-type)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Tests (optional)
# add_subdirectory(tests)

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

# Header-only library
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Examples
add_subdirectory(examples)

# Tests (optional)
# add_subdirectory(tests)

# Install
install(DIRECTORY include/ DESTINATION include)
)";

constexpr const char* cmake_examples = R"(add_executable(example example.cpp)
target_link_libraries(example PRIVATE {{PROJECT_NAME}})
)";

constexpr const char* cmake_tests = R"(# Add your test framework here
# find_package(Catch2 REQUIRED)

add_executable(tests test_main.cpp)
target_link_libraries(tests PRIVATE {{PROJECT_NAME}})
# target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
)";

// =============================================================================
// Source file templates
// =============================================================================

constexpr const char* main_cpp = R"(#include <iostream>

int main() {
    std::cout << "Hello, {{PROJECT_NAME}}!" << std::endl;
    return 0;
}
)";

constexpr const char* lib_hpp = R"(#pragma once

namespace {{PROJECT_NAME}} {

/**
 * @brief Example function
 * @return A greeting string
 */
const char* hello();

} // namespace {{PROJECT_NAME}}
)";

constexpr const char* lib_cpp = R"(#include "{{PROJECT_NAME}}/{{PROJECT_NAME}}.hpp"

namespace {{PROJECT_NAME}} {

const char* hello() {
    return "Hello from {{PROJECT_NAME}}!";
}

} // namespace {{PROJECT_NAME}}
)";

constexpr const char* header_only_hpp = R"(#pragma once

#include <string>

namespace {{PROJECT_NAME}} {

/**
 * @brief Example function
 * @return A greeting string
 */
inline std::string hello() {
    return "Hello from {{PROJECT_NAME}}!";
}

} // namespace {{PROJECT_NAME}}
)";

constexpr const char* example_cpp = R"(#include <iostream>
#include "{{PROJECT_NAME}}/{{PROJECT_NAME}}.hpp"

int main() {
    std::cout << {{PROJECT_NAME}}::hello() << std::endl;
    return 0;
}
)";

constexpr const char* test_main_cpp = R"(// Add your test framework includes here
// #include <catch2/catch_test_macros.hpp>

#include "{{PROJECT_NAME}}/{{PROJECT_NAME}}.hpp"
#include <iostream>

int main() {
    // Basic test
    auto result = {{PROJECT_NAME}}::hello();
    std::cout << "Test: " << result << std::endl;
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

} // namespace fp::templates
