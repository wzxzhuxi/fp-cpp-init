#pragma once

#include <string>
#include <string_view>

namespace fp {

struct RenderContext {
    std::string project_name;
    std::string description;
    std::string cpp_std;
    std::string author;
    std::string year;
    std::string license_name;
};

// 纯函数：模板渲染
auto render(std::string_view tmpl, const RenderContext& ctx) -> std::string;

// 纯函数：获取许可证显示名称
auto get_license_display_name(std::string_view license) -> std::string;

} // namespace fp
