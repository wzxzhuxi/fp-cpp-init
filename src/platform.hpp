#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace fp::platform {

// 跨平台获取 git 用户名
auto get_git_username() -> std::optional<std::string>;

// 跨平台获取当前年份
auto get_current_year() -> std::string;

// 跨平台控制台输出
auto print(std::string_view msg) -> void;
auto print_error(std::string_view msg) -> void;
auto print_success(std::string_view msg) -> void;

// 初始化平台（Windows UTF-8 等）
auto init() -> void;

} // namespace fp::platform
