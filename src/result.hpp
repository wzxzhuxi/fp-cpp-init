#pragma once

#include <string>
#include <string_view>
#include <variant>

namespace fp {

// Result 类型：成功值或错误消息
template <typename T>
class Result {
  public:
    // 构造成功结果
    static auto ok(T value) -> Result { return Result{std::move(value)}; }

    // 构造错误结果
    static auto err(std::string error) -> Result { return Result{Error{std::move(error)}}; }

    // 检查是否成功
    auto is_ok() const -> bool { return std::holds_alternative<T>(data_); }

    auto is_err() const -> bool { return std::holds_alternative<Error>(data_); }

    // 获取值（假定成功）
    auto value() const& -> const T& { return std::get<T>(data_); }

    auto value() && -> T { return std::get<T>(std::move(data_)); }

    // 获取错误消息（假定失败）
    auto error() const -> const std::string& { return std::get<Error>(data_).msg; }

    // map: 转换成功值
    template <typename F>
    auto map(F&& f) const -> Result<decltype(f(std::declval<T>()))> {
        using U = decltype(f(std::declval<T>()));
        if (is_ok()) {
            return Result<U>::ok(f(value()));
        }
        return Result<U>::err(error());
    }

    // and_then: 链式调用（返回 Result 的函数）
    template <typename F>
    auto and_then(F&& f) const -> decltype(f(std::declval<T>())) {
        if (is_ok()) {
            return f(value());
        }
        using U = decltype(f(std::declval<T>()));
        return U::err(error());
    }

    // 获取值或默认值
    auto value_or(T default_value) const -> T {
        if (is_ok()) {
            return value();
        }
        return default_value;
    }

  private:
    struct Error {
        std::string msg;
    };

    std::variant<T, Error> data_;

    explicit Result(T value) : data_(std::move(value)) {}
    explicit Result(Error error) : data_(std::move(error)) {}
};

// void 特化
template <>
class Result<void> {
  public:
    static auto ok() -> Result { return Result{true}; }

    static auto err(std::string error) -> Result { return Result{std::move(error)}; }

    auto is_ok() const -> bool { return success_; }

    auto is_err() const -> bool { return !success_; }

    auto error() const -> const std::string& { return error_; }

  private:
    bool success_;
    std::string error_;

    explicit Result(bool success) : success_(success) {}
    explicit Result(std::string error) : success_(false), error_(std::move(error)) {}
};

} // namespace fp
