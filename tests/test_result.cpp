#include <catch2/catch_test_macros.hpp>
#include <string>

#include "result.hpp"

using namespace fp;

// =============================================================================
// Result<T> Basic Operations
// =============================================================================

TEST_CASE("Result<int> ok value", "[result]") {
    auto r = Result<int>::ok(42);
    REQUIRE(r.is_ok());
    REQUIRE_FALSE(r.is_err());
    REQUIRE(r.value() == 42);
}

TEST_CASE("Result<int> error value", "[result]") {
    auto r = Result<int>::err("error message");
    REQUIRE(r.is_err());
    REQUIRE_FALSE(r.is_ok());
    REQUIRE(r.error() == "error message");
}

TEST_CASE("Result<string> ok value", "[result]") {
    auto r = Result<std::string>::ok("hello");
    REQUIRE(r.is_ok());
    REQUIRE(r.value() == "hello");
}

TEST_CASE("Result<string> error value", "[result]") {
    auto r = Result<std::string>::err("failed");
    REQUIRE(r.is_err());
    REQUIRE(r.error() == "failed");
}

// =============================================================================
// Result<T> map
// =============================================================================

TEST_CASE("Result<T> map transforms ok value", "[result]") {
    auto r = Result<int>::ok(10);
    auto mapped = r.map([](int x) { return x * 2; });
    REQUIRE(mapped.is_ok());
    REQUIRE(mapped.value() == 20);
}

TEST_CASE("Result<T> map propagates error", "[result]") {
    auto r = Result<int>::err("error");
    auto mapped = r.map([](int x) { return x * 2; });
    REQUIRE(mapped.is_err());
    REQUIRE(mapped.error() == "error");
}

TEST_CASE("Result<T> map changes type", "[result]") {
    auto r = Result<int>::ok(42);
    auto mapped = r.map([](int x) { return std::to_string(x); });
    REQUIRE(mapped.is_ok());
    REQUIRE(mapped.value() == "42");
}

// =============================================================================
// Result<T> and_then
// =============================================================================

TEST_CASE("Result<T> and_then chains successful operations", "[result]") {
    auto r = Result<int>::ok(10);
    auto chained = r.and_then([](int x) { return Result<std::string>::ok(std::to_string(x)); });
    REQUIRE(chained.is_ok());
    REQUIRE(chained.value() == "10");
}

TEST_CASE("Result<T> and_then propagates first error", "[result]") {
    auto r = Result<int>::err("first error");
    auto chained = r.and_then([](int x) { return Result<std::string>::ok(std::to_string(x)); });
    REQUIRE(chained.is_err());
    REQUIRE(chained.error() == "first error");
}

TEST_CASE("Result<T> and_then propagates second error", "[result]") {
    auto r = Result<int>::ok(10);
    auto chained = r.and_then([](int) { return Result<std::string>::err("second error"); });
    REQUIRE(chained.is_err());
    REQUIRE(chained.error() == "second error");
}

// =============================================================================
// Result<T> value_or
// =============================================================================

TEST_CASE("Result<T> value_or returns value on ok", "[result]") {
    auto r = Result<int>::ok(42);
    REQUIRE(r.value_or(0) == 42);
}

TEST_CASE("Result<T> value_or returns default on error", "[result]") {
    auto r = Result<int>::err("error");
    REQUIRE(r.value_or(99) == 99);
}

// =============================================================================
// Result<void> Specialization
// =============================================================================

TEST_CASE("Result<void> ok", "[result]") {
    auto r = Result<void>::ok();
    REQUIRE(r.is_ok());
    REQUIRE_FALSE(r.is_err());
}

TEST_CASE("Result<void> error", "[result]") {
    auto r = Result<void>::err("void error");
    REQUIRE(r.is_err());
    REQUIRE_FALSE(r.is_ok());
    REQUIRE(r.error() == "void error");
}

// =============================================================================
// Result<T> Move Semantics
// =============================================================================

TEST_CASE("Result<T> rvalue value access", "[result]") {
    auto get_result = []() { return Result<std::string>::ok("moveable"); };
    std::string moved = std::move(get_result()).value();
    REQUIRE(moved == "moveable");
}
