#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include <string>
#include <vector>

#include "fp-cpp-init/cli.hpp"

using namespace fp;

// Helper to create argv from strings
class ArgvBuilder {
  public:
    ArgvBuilder& add(const char* arg) {
        args_.push_back(arg);
        return *this;
    }

    char** argv() {
        ptrs_.clear();
        for (const auto& arg : args_) {
            ptrs_.push_back(const_cast<char*>(arg));
        }
        return ptrs_.data();
    }

    int argc() const { return static_cast<int>(args_.size()); }

  private:
    std::vector<const char*> args_;
    std::vector<char*> ptrs_;
};

// =============================================================================
// No Arguments / Help / Version
// =============================================================================

TEST_CASE("parse_args with no arguments returns Help", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().command == Command::Help);
}

TEST_CASE("parse_args --help returns Help", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("--help");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().command == Command::Help);
}

TEST_CASE("parse_args -h returns Help", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("-h");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().command == Command::Help);
}

TEST_CASE("parse_args --version returns Version", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("--version");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().command == Command::Version);
}

TEST_CASE("parse_args -v returns Version", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("-v");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().command == Command::Version);
}

// =============================================================================
// New Command
// =============================================================================

TEST_CASE("parse_args new without name returns error", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_err());
}

TEST_CASE("parse_args new with name returns New command", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("myproject");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().command == Command::New);
    REQUIRE(result.value().project_name == "myproject");
}

TEST_CASE("parse_args new --help returns NewHelp", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("--help");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().command == Command::NewHelp);
}

TEST_CASE("parse_args new -h returns NewHelp", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("-h");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().command == Command::NewHelp);
}

TEST_CASE("parse_args new with name starting with dash returns error", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("-invalid");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_err());
}

// =============================================================================
// Type Option
// =============================================================================

TEST_CASE("parse_args validates project type", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--type=invalid");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_err());
}

TEST_CASE("parse_args accepts --type=exe", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--type=exe");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().type == "exe");
}

TEST_CASE("parse_args accepts --type=lib", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--type=lib");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().type == "lib");
}

TEST_CASE("parse_args accepts --type=header", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--type=header");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().type == "header");
}

TEST_CASE("parse_args accepts -t=lib short option", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("-t=lib");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().type == "lib");
}

// =============================================================================
// License Option
// =============================================================================

TEST_CASE("parse_args validates license", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--license=invalid");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_err());
}

TEST_CASE("parse_args accepts --license=mit", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--license=mit");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().license == "mit");
}

TEST_CASE("parse_args accepts --license=apache2", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--license=apache2");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().license == "apache2");
}

TEST_CASE("parse_args accepts --license=gpl3", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--license=gpl3");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().license == "gpl3");
}

TEST_CASE("parse_args accepts --license=bsd3", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--license=bsd3");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().license == "bsd3");
}

TEST_CASE("parse_args accepts --license=none", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--license=none");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().license == "none");
}

TEST_CASE("parse_args accepts -l=mit short option", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("-l=apache2");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().license == "apache2");
}

// =============================================================================
// C++ Standard Option
// =============================================================================

TEST_CASE("parse_args validates C++ standard", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--std=11");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_err());
}

TEST_CASE("parse_args accepts --std=17", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--std=17");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().cpp_std == "17");
}

TEST_CASE("parse_args accepts --std=20", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--std=20");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().cpp_std == "20");
}

TEST_CASE("parse_args accepts --std=23", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--std=23");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().cpp_std == "23");
}

TEST_CASE("parse_args accepts -s=20 short option", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("-s=23");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().cpp_std == "23");
}

// =============================================================================
// Author and Description Options
// =============================================================================

TEST_CASE("parse_args accepts --author option", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--author=John Doe");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().author == "John Doe");
}

TEST_CASE("parse_args accepts -a short option", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("-a=Jane");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().author == "Jane");
}

TEST_CASE("parse_args accepts --desc option", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--desc=My project");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().description == "My project");
}

TEST_CASE("parse_args accepts -d short option", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("-d=Description");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().description == "Description");
}

// =============================================================================
// Boolean Flags
// =============================================================================

TEST_CASE("parse_args --no-ci disables CI", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--no-ci");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE_FALSE(result.value().enable_ci);
}

TEST_CASE("parse_args --no-lint disables lint", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--no-lint");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE_FALSE(result.value().enable_lint);
}

TEST_CASE("parse_args CI and lint enabled by default", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());
    REQUIRE(result.value().enable_ci);
    REQUIRE(result.value().enable_lint);
}

// =============================================================================
// Unknown Options and Commands
// =============================================================================

TEST_CASE("parse_args unknown command returns error", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("unknown");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_err());
}

TEST_CASE("parse_args unknown option returns error", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test").add("--unknown=value");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_err());
}

// =============================================================================
// Multiple Options
// =============================================================================

TEST_CASE("parse_args handles multiple options", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init")
        .add("new")
        .add("myproject")
        .add("--type=lib")
        .add("--license=apache2")
        .add("--std=23")
        .add("--author=Test Author")
        .add("--desc=Test description")
        .add("--no-ci");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());

    auto opts = result.value();
    REQUIRE(opts.command == Command::New);
    REQUIRE(opts.project_name == "myproject");
    REQUIRE(opts.type == "lib");
    REQUIRE(opts.license == "apache2");
    REQUIRE(opts.cpp_std == "23");
    REQUIRE(opts.author == "Test Author");
    REQUIRE(opts.description == "Test description");
    REQUIRE_FALSE(opts.enable_ci);
    REQUIRE(opts.enable_lint);
}

// =============================================================================
// Default Values
// =============================================================================

TEST_CASE("parse_args has correct default values", "[cli]") {
    ArgvBuilder builder;
    builder.add("fp-cpp-init").add("new").add("test");

    auto result = parse_args(builder.argc(), builder.argv());
    REQUIRE(result.is_ok());

    auto opts = result.value();
    REQUIRE(opts.type == "exe");
    REQUIRE(opts.license == "mit");
    REQUIRE(opts.cpp_std == "20");
    REQUIRE(opts.enable_ci);
    REQUIRE(opts.enable_lint);
}

// =============================================================================
// Help Text Functions
// =============================================================================

TEST_CASE("get_help_text returns non-empty string", "[cli]") {
    auto text = get_help_text();
    REQUIRE_FALSE(text.empty());
    REQUIRE(text.find("fp-cpp-init") != std::string::npos);
}

TEST_CASE("get_new_help_text returns non-empty string", "[cli]") {
    auto text = get_new_help_text();
    REQUIRE_FALSE(text.empty());
    REQUIRE(text.find("new") != std::string::npos);
}

TEST_CASE("get_version_text returns version string", "[cli]") {
    auto text = get_version_text();
    REQUIRE_FALSE(text.empty());
    REQUIRE(text.find("0.1.0") != std::string::npos);
}
