#include <catch2/catch_test_macros.hpp>
#include <string>

#include "render.hpp"

using namespace fp;

// =============================================================================
// render() Basic Placeholder Replacement
// =============================================================================

TEST_CASE("render replaces PROJECT_NAME placeholder", "[render]") {
    RenderContext ctx{
        .project_name = "myproject",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "Name: {{PROJECT_NAME}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "Name: myproject");
}

TEST_CASE("render replaces DESCRIPTION placeholder", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "A test project",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "Desc: {{DESCRIPTION}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "Desc: A test project");
}

TEST_CASE("render uses default description when empty", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "{{DESCRIPTION}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "A C++ project");
}

TEST_CASE("render replaces CXX_STD placeholder", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "",
        .cpp_std = "23",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "Standard: C++{{CXX_STD}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "Standard: C++23");
}

TEST_CASE("render replaces AUTHOR placeholder", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "",
        .cpp_std = "20",
        .author = "John Doe",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "By: {{AUTHOR}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "By: John Doe");
}

TEST_CASE("render uses default author when empty", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "{{AUTHOR}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "Author");
}

TEST_CASE("render replaces YEAR placeholder", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "Copyright {{YEAR}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "Copyright 2025");
}

TEST_CASE("render replaces LICENSE_NAME placeholder", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = "MIT License"};

    std::string tmpl = "License: {{LICENSE_NAME}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "License: MIT License");
}

// =============================================================================
// render() Multiple Placeholders
// =============================================================================

TEST_CASE("render handles multiple different placeholders", "[render]") {
    RenderContext ctx{
        .project_name = "mylib",
        .description = "My library",
        .cpp_std = "20",
        .author = "Test Author",
        .year = "2025",
        .license_name = "MIT"};

    std::string tmpl = "# {{PROJECT_NAME}}\n{{DESCRIPTION}}\nBy {{AUTHOR}} ({{YEAR}})";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "# mylib\nMy library\nBy Test Author (2025)");
}

TEST_CASE("render handles multiple occurrences of same placeholder", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "{{PROJECT_NAME}} - {{PROJECT_NAME}} - {{PROJECT_NAME}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "test - test - test");
}

TEST_CASE("render handles template with no placeholders", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "No placeholders here";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "No placeholders here");
}

TEST_CASE("render handles empty template", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "";
    auto result = render(tmpl, ctx);
    REQUIRE(result.empty());
}

// =============================================================================
// render() Edge Cases
// =============================================================================

TEST_CASE("render handles project name with special characters", "[render]") {
    RenderContext ctx{
        .project_name = "my-project_123",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "{{PROJECT_NAME}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "my-project_123");
}

TEST_CASE("render handles description with newlines", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "Line 1\nLine 2",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "{{DESCRIPTION}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "Line 1\nLine 2");
}

TEST_CASE("render handles adjacent placeholders", "[render]") {
    RenderContext ctx{
        .project_name = "test",
        .description = "",
        .cpp_std = "20",
        .author = "Author",
        .year = "2025",
        .license_name = ""};

    std::string tmpl = "{{PROJECT_NAME}}{{YEAR}}{{AUTHOR}}";
    auto result = render(tmpl, ctx);
    REQUIRE(result == "test2025Author");
}

// =============================================================================
// get_license_display_name()
// =============================================================================

TEST_CASE("get_license_display_name returns MIT License", "[render]") {
    REQUIRE(get_license_display_name("mit") == "MIT License");
}

TEST_CASE("get_license_display_name returns Apache License 2.0", "[render]") {
    REQUIRE(get_license_display_name("apache2") == "Apache License 2.0");
}

TEST_CASE("get_license_display_name returns GPL v3", "[render]") {
    REQUIRE(get_license_display_name("gpl3") == "GNU General Public License v3.0");
}

TEST_CASE("get_license_display_name returns BSD 3-Clause", "[render]") {
    REQUIRE(get_license_display_name("bsd3") == "BSD 3-Clause License");
}

TEST_CASE("get_license_display_name returns empty for none", "[render]") {
    REQUIRE(get_license_display_name("none") == "");
}

TEST_CASE("get_license_display_name returns empty for unknown license", "[render]") {
    REQUIRE(get_license_display_name("unknown") == "");
    REQUIRE(get_license_display_name("") == "");
}
