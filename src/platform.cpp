#include "platform.hpp"

#ifdef _WIN32
#include <windows.h>
#define popen _popen
#define pclose _pclose
#else
#include <cstdio>
#endif

#include <ctime>
#include <iostream>

namespace fp::platform {

auto init() -> void {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

auto get_git_username() -> std::optional<std::string> {
    FILE* pipe = popen("git config user.name 2>/dev/null", "r");
    if (!pipe) {
        return std::nullopt;
    }

    char buffer[256];
    std::string result;
    if (fgets(buffer, sizeof(buffer), pipe)) {
        result = buffer;
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
    }
    pclose(pipe);

    return result.empty() ? std::nullopt : std::optional{result};
}

auto get_current_year() -> std::string {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    return std::to_string(1900 + tm->tm_year);
}

auto print(std::string_view msg) -> void {
    std::cout << msg;
}

auto print_error(std::string_view msg) -> void {
#ifdef _WIN32
    std::cerr << "[ERROR] " << msg << "\n";
#else
    std::cerr << "\033[31m✗\033[0m " << msg << "\n";
#endif
}

auto print_success(std::string_view msg) -> void {
#ifdef _WIN32
    std::cout << "[OK] " << msg << "\n";
#else
    std::cout << "\033[32m✓\033[0m " << msg << "\n";
#endif
}

} // namespace fp::platform
