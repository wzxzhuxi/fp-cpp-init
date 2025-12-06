include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# 创建测试库（不含 main.cpp）
add_library(fp-cpp-init-lib STATIC
    ${CMAKE_SOURCE_DIR}/src/cli.cpp
    ${CMAKE_SOURCE_DIR}/src/project.cpp
    ${CMAKE_SOURCE_DIR}/src/render.cpp
    ${CMAKE_SOURCE_DIR}/src/platform.cpp
)
target_include_directories(fp-cpp-init-lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# 跨平台编译选项
if(MSVC)
    target_compile_options(fp-cpp-init-lib PRIVATE /W4 /utf-8)
    target_compile_definitions(fp-cpp-init-lib PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(fp-cpp-init-lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 测试可执行文件
add_executable(tests
    test_result.cpp
    test_cli.cpp
    test_render.cpp
    test_project.cpp
    test_platform.cpp
)
target_link_libraries(tests PRIVATE fp-cpp-init-lib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
