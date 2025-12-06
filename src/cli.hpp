#pragma once

#include <string>

#include "result.hpp"

namespace fp {

// 命令类型
enum class Command { Help, NewHelp, Version, New };

// 不可变选项结构
struct Options {
    Command command;
    std::string project_name;
    std::string type;
    std::string license;
    std::string cpp_std;
    std::string author;
    std::string description;
    bool enable_ci = true;
    bool enable_lint = true;
};

// 纯函数：解析命令行参数
auto parse_args(int argc, char* argv[]) -> Result<Options>;

// 纯函数：生成帮助文本
auto get_help_text() -> std::string;
auto get_new_help_text() -> std::string;
auto get_version_text() -> std::string;

} // namespace fp
