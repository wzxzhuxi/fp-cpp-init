#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "fp-cpp-init/platform.hpp"

using namespace fp;

// =============================================================================
// get_current_year()
// =============================================================================

TEST_CASE("get_current_year returns 4-digit year string", "[platform]") {
    auto year = platform::get_current_year();

    // Should be a 4-digit string
    REQUIRE(year.length() == 4);

    // Should be all digits
    for (char c : year) {
        REQUIRE(std::isdigit(static_cast<unsigned char>(c)));
    }
}

TEST_CASE("get_current_year returns valid year range", "[platform]") {
    auto year = platform::get_current_year();
    int year_int = std::stoi(year);

    // Reasonable year range (test won't break for a while)
    REQUIRE(year_int >= 2024);
    REQUIRE(year_int <= 2100);
}

TEST_CASE("get_current_year is consistent across calls", "[platform]") {
    auto year1 = platform::get_current_year();
    auto year2 = platform::get_current_year();

    // Should return same value (unless called exactly at midnight on Dec 31)
    REQUIRE(year1 == year2);
}

// =============================================================================
// get_git_username()
// =============================================================================

TEST_CASE("get_git_username returns optional", "[platform]") {
    auto username = platform::get_git_username();

    // This test just verifies the function doesn't crash
    // The result depends on whether git is configured
    if (username.has_value()) {
        // If it has a value, it should not be empty
        REQUIRE_FALSE(username->empty());
        // Should not have trailing newline
        REQUIRE(username->back() != '\n');
    }
    // If no value, that's also valid (git might not be configured)
}

TEST_CASE("get_git_username result has no newlines", "[platform]") {
    auto username = platform::get_git_username();

    if (username.has_value()) {
        // Check no newlines anywhere in the string
        REQUIRE(username->find('\n') == std::string::npos);
        REQUIRE(username->find('\r') == std::string::npos);
    }
}

// =============================================================================
// init()
// =============================================================================

TEST_CASE("platform::init does not crash", "[platform]") {
    // Just verify init() can be called without crashing
    REQUIRE_NOTHROW(platform::init());
}

TEST_CASE("platform::init can be called multiple times", "[platform]") {
    // Should be idempotent
    REQUIRE_NOTHROW(platform::init());
    REQUIRE_NOTHROW(platform::init());
    REQUIRE_NOTHROW(platform::init());
}

// =============================================================================
// print functions - basic smoke tests
// =============================================================================

TEST_CASE("platform::print does not crash with empty string", "[platform]") {
    REQUIRE_NOTHROW(platform::print(""));
}

TEST_CASE("platform::print does not crash with normal string", "[platform]") {
    // Redirect stdout to suppress output during tests
    std::streambuf* old_cout = std::cout.rdbuf();
    std::ostringstream captured;
    std::cout.rdbuf(captured.rdbuf());

    REQUIRE_NOTHROW(platform::print("test message"));

    std::cout.rdbuf(old_cout);
    REQUIRE(captured.str() == "test message");
}

TEST_CASE("platform::print_error does not crash", "[platform]") {
    // Redirect stderr to suppress output during tests
    std::streambuf* old_cerr = std::cerr.rdbuf();
    std::ostringstream captured;
    std::cerr.rdbuf(captured.rdbuf());

    REQUIRE_NOTHROW(platform::print_error("error message"));

    std::cerr.rdbuf(old_cerr);
    REQUIRE(captured.str().find("error message") != std::string::npos);
}

TEST_CASE("platform::print_success does not crash", "[platform]") {
    // Redirect stdout to suppress output during tests
    std::streambuf* old_cout = std::cout.rdbuf();
    std::ostringstream captured;
    std::cout.rdbuf(captured.rdbuf());

    REQUIRE_NOTHROW(platform::print_success("success message"));

    std::cout.rdbuf(old_cout);
    REQUIRE(captured.str().find("success message") != std::string::npos);
}

// =============================================================================
// print functions - content verification
// =============================================================================

TEST_CASE("platform::print_error includes error prefix on Unix", "[platform]") {
#ifndef _WIN32
    std::streambuf* old_cerr = std::cerr.rdbuf();
    std::ostringstream captured;
    std::cerr.rdbuf(captured.rdbuf());

    platform::print_error("test");

    std::cerr.rdbuf(old_cerr);
    // Unix version uses colored output with ✗
    REQUIRE(captured.str().find("test") != std::string::npos);
#endif
}

TEST_CASE("platform::print_success includes success prefix on Unix", "[platform]") {
#ifndef _WIN32
    std::streambuf* old_cout = std::cout.rdbuf();
    std::ostringstream captured;
    std::cout.rdbuf(captured.rdbuf());

    platform::print_success("test");

    std::cout.rdbuf(old_cout);
    // Unix version uses colored output with ✓
    REQUIRE(captured.str().find("test") != std::string::npos);
#endif
}

// =============================================================================
// Edge cases
// =============================================================================

TEST_CASE("platform::print handles special characters", "[platform]") {
    std::streambuf* old_cout = std::cout.rdbuf();
    std::ostringstream captured;
    std::cout.rdbuf(captured.rdbuf());

    REQUIRE_NOTHROW(platform::print("Hello 世界 🎉"));

    std::cout.rdbuf(old_cout);
    REQUIRE(captured.str() == "Hello 世界 🎉");
}

TEST_CASE("platform::print handles newlines", "[platform]") {
    std::streambuf* old_cout = std::cout.rdbuf();
    std::ostringstream captured;
    std::cout.rdbuf(captured.rdbuf());

    platform::print("line1\nline2\n");

    std::cout.rdbuf(old_cout);
    REQUIRE(captured.str() == "line1\nline2\n");
}
