#include "cli.hpp"

#include <cstring>
#include <sstream>

#include "platform.hpp"

namespace fp {

namespace {

auto starts_with(const char* str, const char* prefix) -> bool {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

auto get_option_value(const char* arg, const char* long_opt, const char* short_opt) -> std::string {
    size_t long_len = strlen(long_opt);
    size_t short_len = strlen(short_opt);

    if (starts_with(arg, long_opt) && arg[long_len] == '=') {
        return arg + long_len + 1;
    }
    if (starts_with(arg, short_opt) && arg[short_len] == '=') {
        return arg + short_len + 1;
    }
    return "";
}

auto is_valid_type(const std::string& type) -> bool {
    return type == "exe" || type == "lib" || type == "header";
}

auto is_valid_license(const std::string& license) -> bool {
    return license == "mit" || license == "apache2" || license == "gpl3" || license == "bsd3" ||
           license == "none";
}

auto is_valid_std(const std::string& std) -> bool {
    return std == "17" || std == "20" || std == "23";
}

} // anonymous namespace

auto parse_args(int argc, char* argv[]) -> Result<Options> {
    Options opts{.command = Command::Help,
                 .project_name = "",
                 .type = "exe",
                 .license = "mit",
                 .cpp_std = "20",
                 .author = platform::get_git_username().value_or(""),
                 .description = "",
                 .enable_ci = true,
                 .enable_lint = true};

    if (argc < 2) {
        return Result<Options>::ok(opts);
    }

    std::string first_arg = argv[1];

    // 全局选项
    if (first_arg == "--help" || first_arg == "-h") {
        opts.command = Command::Help;
        return Result<Options>::ok(opts);
    }
    if (first_arg == "--version" || first_arg == "-v") {
        opts.command = Command::Version;
        return Result<Options>::ok(opts);
    }

    // new 命令
    if (first_arg == "new") {
        opts.command = Command::New;

        if (argc < 3) {
            return Result<Options>::err("Error: Project name required.\n"
                                        "Usage: fp-cpp-init new <project-name> [options]");
        }

        std::string second_arg = argv[2];

        if (second_arg == "--help" || second_arg == "-h") {
            opts.command = Command::NewHelp;
            return Result<Options>::ok(opts);
        }

        if (second_arg[0] == '-') {
            return Result<Options>::err("Error: Project name cannot start with '-'");
        }

        opts.project_name = second_arg;

        // 解析剩余选项
        for (int i = 3; i < argc; ++i) {
            const char* arg = argv[i];
            std::string val;

            if ((val = get_option_value(arg, "--type", "-t")) != "") {
                if (!is_valid_type(val)) {
                    return Result<Options>::err("Error: Invalid type '" + val +
                                                "'. Must be: exe, lib, or header");
                }
                opts.type = val;
            } else if ((val = get_option_value(arg, "--license", "-l")) != "") {
                if (!is_valid_license(val)) {
                    return Result<Options>::err("Error: Invalid license '" + val +
                                                "'. Must be: mit, apache2, gpl3, bsd3, or none");
                }
                opts.license = val;
            } else if ((val = get_option_value(arg, "--std", "-s")) != "") {
                if (!is_valid_std(val)) {
                    return Result<Options>::err("Error: Invalid C++ standard '" + val +
                                                "'. Must be: 17, 20, or 23");
                }
                opts.cpp_std = val;
            } else if ((val = get_option_value(arg, "--author", "-a")) != "") {
                opts.author = val;
            } else if ((val = get_option_value(arg, "--desc", "-d")) != "") {
                opts.description = val;
            } else if (strcmp(arg, "--no-ci") == 0) {
                opts.enable_ci = false;
            } else if (strcmp(arg, "--no-lint") == 0) {
                opts.enable_lint = false;
            } else {
                return Result<Options>::err("Error: Unknown option '" + std::string(arg) + "'");
            }
        }

        return Result<Options>::ok(opts);
    }

    // 未知命令
    return Result<Options>::err("Error: Unknown command '" + first_arg + "'");
}

auto get_help_text() -> std::string {
    return R"(fp-cpp-init - C++ Project Scaffolding Tool

USAGE:
    fp-cpp-init <COMMAND> [OPTIONS]

COMMANDS:
    new <name>    Create a new C++ project
    --help, -h    Show this help message
    --version, -v Show version information

EXAMPLES:
    fp-cpp-init new myproject
    fp-cpp-init new myproject --type=lib --license=apache2
    fp-cpp-init new --help

For more information about a command, use:
    fp-cpp-init <COMMAND> --help
)";
}

auto get_new_help_text() -> std::string {
    return R"(fp-cpp-init new - Create a new C++ project

USAGE:
    fp-cpp-init new <project-name> [OPTIONS]

OPTIONS:
    -t, --type=<TYPE>       Project type [default: exe]
                            Values: exe, lib, header
    -l, --license=<LICENSE> License type [default: mit]
                            Values: mit, apache2, gpl3, bsd3, none
    -s, --std=<STANDARD>    C++ standard [default: 20]
                            Values: 17, 20, 23
    -a, --author=<NAME>     Author name [default: git config user.name]
    -d, --desc=<TEXT>       Project description

    --no-ci                 Disable GitHub Actions CI/CD
    --no-lint               Disable .clang-format and .clang-tidy

PROJECT TYPES:
    exe     Executable application (with main.cpp)
    lib     Static library (with .hpp and .cpp)
    header  Header-only library

EXAMPLES:
    fp-cpp-init new myapp
    fp-cpp-init new mylib --type=lib --std=20
    fp-cpp-init new myheader --type=header --license=apache2
    fp-cpp-init new myproject --author="John Doe" --desc="My awesome project"
    fp-cpp-init new myapp --no-ci --no-lint
)";
}

auto get_version_text() -> std::string {
    return "fp-cpp-init 0.1.0\n";
}

} // namespace fp
