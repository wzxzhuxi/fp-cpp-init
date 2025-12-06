#include "render.hpp"

namespace fp {

auto render(std::string_view tmpl, const RenderContext& ctx) -> std::string {
    std::string result(tmpl);

    auto replace_all = [&result](const std::string& from, const std::string& to) {
        size_t pos = 0;
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
    };

    replace_all("{{PROJECT_NAME}}", ctx.project_name);
    replace_all("{{DESCRIPTION}}", ctx.description.empty() ? "A C++ project" : ctx.description);
    replace_all("{{CXX_STD}}", ctx.cpp_std);
    replace_all("{{AUTHOR}}", ctx.author.empty() ? "Author" : ctx.author);
    replace_all("{{YEAR}}", ctx.year);
    replace_all("{{LICENSE_NAME}}", ctx.license_name);

    return result;
}

auto get_license_display_name(std::string_view license) -> std::string {
    if (license == "mit") return "MIT License";
    if (license == "apache2") return "Apache License 2.0";
    if (license == "gpl3") return "GNU General Public License v3.0";
    if (license == "bsd3") return "BSD 3-Clause License";
    return "";
}

} // namespace fp
