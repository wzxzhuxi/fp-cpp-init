cmake_minimum_required(VERSION 3.20)

project(
    fp-cpp-init
    VERSION 0.1.0
    DESCRIPTION "C++ Project Scaffolding Tool"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(fp-cpp-init
    src/main.cpp
    src/cli.cpp
    src/platform.cpp
    src/project.cpp
    src/render.cpp
)

target_include_directories(fp-cpp-init PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# 跨平台编译选项
if(MSVC)
    target_compile_options(fp-cpp-init PRIVATE /W4 /utf-8)
    target_compile_definitions(fp-cpp-init PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(fp-cpp-init PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 安装
install(TARGETS fp-cpp-init DESTINATION bin)

# 测试
option(BUILD_TESTING "Build tests" OFF)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
