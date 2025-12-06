#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <string>

#include "fp-cpp-init/project.hpp"

using namespace fp;

// Helper to check if a project has a specific file
auto has_file(const ProjectFiles& project, const std::string& path) -> bool {
    return std::any_of(project.files.begin(), project.files.end(),
                       [&path](const FileEntry& f) { return f.path.string() == path; });
}

// Helper to check if a project has a specific directory
auto has_dir(const ProjectFiles& project, const std::string& path) -> bool {
    return std::any_of(project.directories.begin(), project.directories.end(),
                       [&path](const std::filesystem::path& d) { return d.string() == path; });
}

// Helper to get file content
auto get_file_content(const ProjectFiles& project, const std::string& path) -> std::string {
    for (const auto& f : project.files) {
        if (f.path.string() == path) {
            return f.content;
        }
    }
    return "";
}

// =============================================================================
// Exe Project Generation
// =============================================================================

TEST_CASE("generate_project creates exe project structure", "[project]") {
    Options opts{};
    opts.command = Command::New;
    opts.project_name = "testexe";
    opts.type = "exe";
    opts.license = "mit";
    opts.cpp_std = "20";
    opts.enable_ci = true;
    opts.enable_lint = true;

    RenderContext ctx{
        .project_name = "testexe",
        .description = "Test project",
        .cpp_std = "20",
        .author = "Test",
        .year = "2025",
        .license_name = "MIT License"};

    auto project = generate_project(opts, ctx);

    // Check directories
    REQUIRE(has_dir(project, "testexe/src"));
    REQUIRE(has_dir(project, "testexe/include/testexe"));
    REQUIRE(has_dir(project, "testexe/tests"));
    REQUIRE(has_dir(project, "testexe/.github/workflows"));

    // Check files
    REQUIRE(has_file(project, "testexe/CMakeLists.txt"));
    REQUIRE(has_file(project, "testexe/src/main.cpp"));
    REQUIRE(has_file(project, "testexe/include/testexe/result.hpp"));
    REQUIRE(has_file(project, "testexe/README.md"));
    REQUIRE(has_file(project, "testexe/.gitignore"));
    REQUIRE(has_file(project, "testexe/.clang-format"));
    REQUIRE(has_file(project, "testexe/.clang-tidy"));
    REQUIRE(has_file(project, "testexe/LICENSE"));
    REQUIRE(has_file(project, "testexe/.github/workflows/ci.yml"));
    REQUIRE(has_file(project, "testexe/.github/workflows/release.yml"));
}

TEST_CASE("exe project CMakeLists.txt contains project name", "[project]") {
    Options opts{};
    opts.project_name = "myexe";
    opts.type = "exe";
    opts.license = "none";
    opts.cpp_std = "23";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{
        .project_name = "myexe",
        .description = "",
        .cpp_std = "23",
        .author = "",
        .year = "2025",
        .license_name = ""};

    auto project = generate_project(opts, ctx);
    auto cmake = get_file_content(project, "myexe/CMakeLists.txt");

    REQUIRE(cmake.find("myexe") != std::string::npos);
    REQUIRE(cmake.find("CMAKE_CXX_STANDARD 23") != std::string::npos);
}

// =============================================================================
// Lib Project Generation
// =============================================================================

TEST_CASE("generate_project creates lib project structure", "[project]") {
    Options opts{};
    opts.project_name = "testlib";
    opts.type = "lib";
    opts.license = "apache2";
    opts.cpp_std = "20";
    opts.enable_ci = true;
    opts.enable_lint = true;

    RenderContext ctx{
        .project_name = "testlib",
        .description = "Test library",
        .cpp_std = "20",
        .author = "Test",
        .year = "2025",
        .license_name = "Apache License 2.0"};

    auto project = generate_project(opts, ctx);

    // Check directories
    REQUIRE(has_dir(project, "testlib/src"));
    REQUIRE(has_dir(project, "testlib/include/testlib"));
    REQUIRE(has_dir(project, "testlib/tests"));

    // Check lib-specific files
    REQUIRE(has_file(project, "testlib/include/testlib/testlib.hpp"));
    REQUIRE(has_file(project, "testlib/include/testlib/result.hpp"));
    REQUIRE(has_file(project, "testlib/src/testlib.cpp"));
    REQUIRE(has_file(project, "testlib/tests/CMakeLists.txt"));
    REQUIRE(has_file(project, "testlib/tests/test_main.cpp"));

    // Lib should NOT have release.yml (only exe has that)
    REQUIRE_FALSE(has_file(project, "testlib/.github/workflows/release.yml"));
}

TEST_CASE("lib project has proper header and source files", "[project]") {
    Options opts{};
    opts.project_name = "mylib";
    opts.type = "lib";
    opts.license = "none";
    opts.cpp_std = "20";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{
        .project_name = "mylib",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    auto project = generate_project(opts, ctx);

    auto hpp = get_file_content(project, "mylib/include/mylib/mylib.hpp");
    auto cpp = get_file_content(project, "mylib/src/mylib.cpp");

    REQUIRE(hpp.find("namespace mylib") != std::string::npos);
    REQUIRE(cpp.find("#include \"mylib/mylib.hpp\"") != std::string::npos);
}

// =============================================================================
// Header-only Project Generation
// =============================================================================

TEST_CASE("generate_project creates header project structure", "[project]") {
    Options opts{};
    opts.project_name = "testheader";
    opts.type = "header";
    opts.license = "mit";
    opts.cpp_std = "20";
    opts.enable_ci = true;
    opts.enable_lint = true;

    RenderContext ctx{
        .project_name = "testheader",
        .description = "Test header-only",
        .cpp_std = "20",
        .author = "Test",
        .year = "2025",
        .license_name = "MIT License"};

    auto project = generate_project(opts, ctx);

    // Check directories
    REQUIRE(has_dir(project, "testheader/examples"));
    REQUIRE(has_dir(project, "testheader/include/testheader"));
    REQUIRE(has_dir(project, "testheader/tests"));

    // Check header-specific files
    REQUIRE(has_file(project, "testheader/include/testheader/testheader.hpp"));
    REQUIRE(has_file(project, "testheader/examples/CMakeLists.txt"));
    REQUIRE(has_file(project, "testheader/examples/example.cpp"));
    REQUIRE(has_file(project, "testheader/tests/CMakeLists.txt"));
    REQUIRE(has_file(project, "testheader/tests/test_main.cpp"));

    // Header should NOT have src directory
    REQUIRE_FALSE(has_dir(project, "testheader/src"));
    // Header should NOT have result.hpp (it's embedded in main header)
    REQUIRE_FALSE(has_file(project, "testheader/include/testheader/result.hpp"));
}

TEST_CASE("header project main file is self-contained", "[project]") {
    Options opts{};
    opts.project_name = "myheader";
    opts.type = "header";
    opts.license = "none";
    opts.cpp_std = "20";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{
        .project_name = "myheader",
        .description = "",
        .cpp_std = "20",
        .author = "",
        .year = "2025",
        .license_name = ""};

    auto project = generate_project(opts, ctx);

    auto hpp = get_file_content(project, "myheader/include/myheader/myheader.hpp");

    // Header-only should have Result class embedded
    REQUIRE(hpp.find("class Result") != std::string::npos);
    REQUIRE(hpp.find("inline auto greet") != std::string::npos);
}

// =============================================================================
// Flag Options
// =============================================================================

TEST_CASE("no-ci flag disables CI files", "[project]") {
    Options opts{};
    opts.project_name = "test";
    opts.type = "exe";
    opts.license = "mit";
    opts.cpp_std = "20";
    opts.enable_ci = false;
    opts.enable_lint = true;

    RenderContext ctx{.project_name = "test", .cpp_std = "20", .year = "2025"};

    auto project = generate_project(opts, ctx);

    REQUIRE_FALSE(has_dir(project, "test/.github/workflows"));
    REQUIRE_FALSE(has_file(project, "test/.github/workflows/ci.yml"));
    REQUIRE_FALSE(has_file(project, "test/.github/workflows/release.yml"));
}

TEST_CASE("no-lint flag disables lint files", "[project]") {
    Options opts{};
    opts.project_name = "test";
    opts.type = "exe";
    opts.license = "mit";
    opts.cpp_std = "20";
    opts.enable_ci = true;
    opts.enable_lint = false;

    RenderContext ctx{.project_name = "test", .cpp_std = "20", .year = "2025"};

    auto project = generate_project(opts, ctx);

    REQUIRE_FALSE(has_file(project, "test/.clang-format"));
    REQUIRE_FALSE(has_file(project, "test/.clang-tidy"));
}

TEST_CASE("license none disables license file", "[project]") {
    Options opts{};
    opts.project_name = "test";
    opts.type = "exe";
    opts.license = "none";
    opts.cpp_std = "20";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{.project_name = "test", .cpp_std = "20", .year = "2025"};

    auto project = generate_project(opts, ctx);

    REQUIRE_FALSE(has_file(project, "test/LICENSE"));
}

// =============================================================================
// License Generation
// =============================================================================

TEST_CASE("MIT license is generated correctly", "[project]") {
    Options opts{};
    opts.project_name = "test";
    opts.type = "exe";
    opts.license = "mit";
    opts.cpp_std = "20";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{
        .project_name = "test",
        .cpp_std = "20",
        .author = "John Doe",
        .year = "2025"};

    auto project = generate_project(opts, ctx);
    auto license = get_file_content(project, "test/LICENSE");

    REQUIRE(license.find("MIT License") != std::string::npos);
    REQUIRE(license.find("2025") != std::string::npos);
    REQUIRE(license.find("John Doe") != std::string::npos);
}

TEST_CASE("Apache2 license is generated correctly", "[project]") {
    Options opts{};
    opts.project_name = "test";
    opts.type = "exe";
    opts.license = "apache2";
    opts.cpp_std = "20";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{
        .project_name = "test",
        .cpp_std = "20",
        .author = "Test Author",
        .year = "2025"};

    auto project = generate_project(opts, ctx);
    auto license = get_file_content(project, "test/LICENSE");

    REQUIRE(license.find("Apache License") != std::string::npos);
}

TEST_CASE("GPL3 license is generated correctly", "[project]") {
    Options opts{};
    opts.project_name = "test";
    opts.type = "exe";
    opts.license = "gpl3";
    opts.cpp_std = "20";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{.project_name = "test", .cpp_std = "20", .year = "2025"};

    auto project = generate_project(opts, ctx);
    auto license = get_file_content(project, "test/LICENSE");

    REQUIRE(license.find("GNU General Public License") != std::string::npos);
}

TEST_CASE("BSD3 license is generated correctly", "[project]") {
    Options opts{};
    opts.project_name = "test";
    opts.type = "exe";
    opts.license = "bsd3";
    opts.cpp_std = "20";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{.project_name = "test", .cpp_std = "20", .year = "2025"};

    auto project = generate_project(opts, ctx);
    auto license = get_file_content(project, "test/LICENSE");

    REQUIRE(license.find("BSD 3-Clause License") != std::string::npos);
}

// =============================================================================
// Content Rendering
// =============================================================================

TEST_CASE("generated files contain rendered placeholders", "[project]") {
    Options opts{};
    opts.project_name = "rendered";
    opts.type = "exe";
    opts.license = "none";
    opts.cpp_std = "23";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{
        .project_name = "rendered",
        .description = "My Description",
        .cpp_std = "23",
        .author = "Author Name",
        .year = "2025",
        .license_name = "MIT License"};

    auto project = generate_project(opts, ctx);

    // CMakeLists.txt should have rendered values
    auto cmake = get_file_content(project, "rendered/CMakeLists.txt");
    REQUIRE(cmake.find("rendered") != std::string::npos);
    REQUIRE(cmake.find("My Description") != std::string::npos);
    REQUIRE(cmake.find("CMAKE_CXX_STANDARD 23") != std::string::npos);

    // main.cpp should have namespace
    auto main = get_file_content(project, "rendered/src/main.cpp");
    REQUIRE(main.find("namespace rendered") != std::string::npos);

    // result.hpp should have namespace
    auto result = get_file_content(project, "rendered/include/rendered/result.hpp");
    REQUIRE(result.find("namespace rendered") != std::string::npos);
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST_CASE("project name with hyphens works correctly", "[project]") {
    Options opts{};
    opts.project_name = "my-cool-project";
    opts.type = "exe";
    opts.license = "none";
    opts.cpp_std = "20";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{
        .project_name = "my-cool-project",
        .cpp_std = "20",
        .year = "2025"};

    auto project = generate_project(opts, ctx);

    REQUIRE(has_dir(project, "my-cool-project/src"));
    REQUIRE(has_file(project, "my-cool-project/CMakeLists.txt"));
    REQUIRE(has_file(project, "my-cool-project/include/my-cool-project/result.hpp"));
}

TEST_CASE("project name with underscores works correctly", "[project]") {
    Options opts{};
    opts.project_name = "my_project";
    opts.type = "lib";
    opts.license = "none";
    opts.cpp_std = "20";
    opts.enable_ci = false;
    opts.enable_lint = false;

    RenderContext ctx{
        .project_name = "my_project",
        .cpp_std = "20",
        .year = "2025"};

    auto project = generate_project(opts, ctx);

    REQUIRE(has_file(project, "my_project/include/my_project/my_project.hpp"));
    REQUIRE(has_file(project, "my_project/src/my_project.cpp"));
}
