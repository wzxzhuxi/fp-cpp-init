#include <filesystem>
#include <fstream>
#include <iostream>

#include "fp-cpp-init/cli.hpp"
#include "fp-cpp-init/platform.hpp"
#include "fp-cpp-init/project.hpp"
#include "fp-cpp-init/render.hpp"

namespace fs = std::filesystem;

namespace {

// 副作用：写入项目文件到磁盘
auto write_project(const fp::ProjectFiles& project) -> fp::Result<void> {
    // 创建目录
    for (const auto& dir : project.directories) {
        try {
            fs::create_directories(dir);
        } catch (const std::exception& e) {
            return fp::Result<void>::err("Error creating directory: " + std::string(e.what()));
        }
    }

    // 写入文件
    for (const auto& file : project.files) {
        try {
            // 确保父目录存在
            if (file.path.has_parent_path()) {
                fs::create_directories(file.path.parent_path());
            }

            std::ofstream ofs(file.path);
            if (!ofs) {
                return fp::Result<void>::err("Error creating file: " + file.path.string());
            }
            ofs << file.content;

            fp::platform::print_success("Created: " + file.path.string());
        } catch (const std::exception& e) {
            return fp::Result<void>::err("Error writing file: " + std::string(e.what()));
        }
    }

    return fp::Result<void>::ok();
}

// 副作用：打印下一步提示
auto print_next_steps(const fp::Options& opts) -> void {
    std::cout << "\nNext steps:\n";
    std::cout << "  cd " << opts.project_name << "\n";
    std::cout << "  cmake -B build -G Ninja\n";
    std::cout << "  cmake --build build\n";

    if (opts.type == "exe") {
        std::cout << "  ./build/" << opts.project_name << "\n";
    } else if (opts.type == "header") {
        std::cout << "  ./build/examples/example\n";
    }
}

} // anonymous namespace

int main(int argc, char* argv[]) {
    // 初始化平台
    fp::platform::init();

    // 解析命令行参数（纯函数）
    auto result = fp::parse_args(argc, argv);

    if (result.is_err()) {
        fp::platform::print_error(result.error());
        return 1;
    }

    const auto& opts = result.value();

    // 处理命令
    switch (opts.command) {
    case fp::Command::Help:
        std::cout << fp::get_help_text();
        return 0;

    case fp::Command::NewHelp:
        std::cout << fp::get_new_help_text();
        return 0;

    case fp::Command::Version:
        std::cout << fp::get_version_text();
        return 0;

    case fp::Command::New: {
        // 检查目录是否已存在
        if (fs::exists(opts.project_name)) {
            fp::platform::print_error("Directory '" + opts.project_name + "' already exists.");
            return 1;
        }

        std::cout << "Creating project '" << opts.project_name << "'...\n\n";

        // 构建渲染上下文（纯数据）
        fp::RenderContext ctx{.project_name = opts.project_name,
                              .description = opts.description,
                              .cpp_std = opts.cpp_std,
                              .author = opts.author,
                              .year = fp::platform::get_current_year(),
                              .license_name = fp::get_license_display_name(opts.license)};

        // 生成项目结构（纯函数）
        auto project = fp::generate_project(opts, ctx);

        // 写入文件（副作用）
        auto write_result = write_project(project);

        if (write_result.is_err()) {
            fp::platform::print_error(write_result.error());
            return 1;
        }

        fp::platform::print_success("Project created successfully!");
        print_next_steps(opts);

        return 0;
    }
    }

    return 1;
}
