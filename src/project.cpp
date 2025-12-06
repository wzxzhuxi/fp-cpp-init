#include "project.hpp"
#include "templates.hpp"
#include "render.hpp"

namespace fp {

namespace {

auto add_common_files(
    ProjectFiles& project,
    const Options& opts,
    const RenderContext& ctx
) -> void {
    // 通用目录
    project.directories.push_back(opts.project_name + "/include/" + opts.project_name);
    project.directories.push_back(opts.project_name + "/tests");

    // .gitignore
    project.files.push_back({
        opts.project_name + "/.gitignore",
        std::string(templates::gitignore)
    });

    // .clang-format
    project.files.push_back({
        opts.project_name + "/.clang-format",
        std::string(templates::clang_format)
    });

    // .clang-tidy
    project.files.push_back({
        opts.project_name + "/.clang-tidy",
        std::string(templates::clang_tidy)
    });

    // LICENSE
    if (opts.license != "none") {
        const char* license_tmpl = nullptr;
        if (opts.license == "mit") license_tmpl = templates::license_mit;
        else if (opts.license == "apache2") license_tmpl = templates::license_apache2;
        else if (opts.license == "gpl3") license_tmpl = templates::license_gpl3;
        else if (opts.license == "bsd3") license_tmpl = templates::license_bsd3;

        if (license_tmpl) {
            project.files.push_back({
                opts.project_name + "/LICENSE",
                render(license_tmpl, ctx)
            });
        }
    }
}

auto generate_exe_project(const Options& opts, const RenderContext& ctx) -> ProjectFiles {
    ProjectFiles project;

    // 目录
    project.directories.push_back(opts.project_name + "/src");
    add_common_files(project, opts, ctx);

    // CMakeLists.txt
    project.files.push_back({
        opts.project_name + "/CMakeLists.txt",
        render(templates::cmake_exe, ctx)
    });

    // src/main.cpp
    project.files.push_back({
        opts.project_name + "/src/main.cpp",
        render(templates::main_cpp, ctx)
    });

    // README.md
    project.files.push_back({
        opts.project_name + "/README.md",
        render(templates::readme, ctx)
    });

    return project;
}

auto generate_lib_project(const Options& opts, const RenderContext& ctx) -> ProjectFiles {
    ProjectFiles project;

    // 目录
    project.directories.push_back(opts.project_name + "/src");
    add_common_files(project, opts, ctx);

    // CMakeLists.txt
    project.files.push_back({
        opts.project_name + "/CMakeLists.txt",
        render(templates::cmake_lib, ctx)
    });

    // tests/CMakeLists.txt
    project.files.push_back({
        opts.project_name + "/tests/CMakeLists.txt",
        render(templates::cmake_tests, ctx)
    });

    // include/project/project.hpp
    project.files.push_back({
        opts.project_name + "/include/" + opts.project_name + "/" + opts.project_name + ".hpp",
        render(templates::lib_hpp, ctx)
    });

    // src/project.cpp
    project.files.push_back({
        opts.project_name + "/src/" + opts.project_name + ".cpp",
        render(templates::lib_cpp, ctx)
    });

    // tests/test_main.cpp
    project.files.push_back({
        opts.project_name + "/tests/test_main.cpp",
        render(templates::test_main_cpp, ctx)
    });

    // README.md
    project.files.push_back({
        opts.project_name + "/README.md",
        render(templates::readme_lib, ctx)
    });

    return project;
}

auto generate_header_project(const Options& opts, const RenderContext& ctx) -> ProjectFiles {
    ProjectFiles project;

    // 目录
    project.directories.push_back(opts.project_name + "/examples");
    add_common_files(project, opts, ctx);

    // CMakeLists.txt
    project.files.push_back({
        opts.project_name + "/CMakeLists.txt",
        render(templates::cmake_header, ctx)
    });

    // examples/CMakeLists.txt
    project.files.push_back({
        opts.project_name + "/examples/CMakeLists.txt",
        render(templates::cmake_examples, ctx)
    });

    // tests/CMakeLists.txt
    project.files.push_back({
        opts.project_name + "/tests/CMakeLists.txt",
        render(templates::cmake_tests, ctx)
    });

    // include/project/project.hpp
    project.files.push_back({
        opts.project_name + "/include/" + opts.project_name + "/" + opts.project_name + ".hpp",
        render(templates::header_only_hpp, ctx)
    });

    // examples/example.cpp
    project.files.push_back({
        opts.project_name + "/examples/example.cpp",
        render(templates::example_cpp, ctx)
    });

    // tests/test_main.cpp
    project.files.push_back({
        opts.project_name + "/tests/test_main.cpp",
        render(templates::test_main_cpp, ctx)
    });

    // README.md
    project.files.push_back({
        opts.project_name + "/README.md",
        render(templates::readme_lib, ctx)
    });

    return project;
}

} // anonymous namespace

auto generate_project(const Options& opts, const RenderContext& ctx) -> ProjectFiles {
    if (opts.type == "lib") {
        return generate_lib_project(opts, ctx);
    }
    if (opts.type == "header") {
        return generate_header_project(opts, ctx);
    }
    return generate_exe_project(opts, ctx);
}

} // namespace fp
