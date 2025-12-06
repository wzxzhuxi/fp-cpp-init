#pragma once

#include "cli.hpp"
#include "render.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace fp {

// 纯数据：表示要创建的文件
struct FileEntry {
    std::filesystem::path path;
    std::string content;
};

// 纯数据：整个项目结构
struct ProjectFiles {
    std::vector<std::filesystem::path> directories;
    std::vector<FileEntry> files;
};

// 纯函数：根据选项生成项目结构（不执行任何 IO）
auto generate_project(const Options& opts, const RenderContext& ctx) -> ProjectFiles;

} // namespace fp
