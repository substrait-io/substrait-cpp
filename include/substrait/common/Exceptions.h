/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <fmt/format.h>
#include <memory>
#include <utility>

namespace io::substrait::common {
namespace error_code {

//====================== User Error Codes ======================:

// An error raised when an argument verification fails
inline constexpr const char* kInvalidArgument = "INVALID_ARGUMENT";

// An error raised when a requested operation is not supported.
inline constexpr const char* kUnsupported = "UNSUPPORTED";

//====================== Runtime Error Codes ======================:

// An error raised when the current state of a component is invalid.
inline constexpr const char* kInvalidState = "INVALID_STATE";

// An error raised when unreachable code point was executed.
inline constexpr const char* kUnreachableCode = "UNREACHABLE_CODE";

// An error raised when a requested operation is not implemented.
inline constexpr const char* kNotImplemented = "NOT_IMPLEMENTED";

} // namespace error_code

class SubstraitException : public std::exception {
 public:
  enum class Type {
    // Errors where the root cause of the problem is either because of bad input
    // or an unsupported pattern of use are classified with USER. Examples
    // of errors in this category include syntax errors, unavailable names or
    // objects.
    kUser = 0,

    // Errors where the root cause of the problem is some unreliable aspect of
    // the system are classified with SYSTEM.
    kSystem = 1
  };
  SubstraitException(
      const char* file,
      size_t line,
      const char* function,
      const std::string& exceptionCode,
      const std::string& exceptionMessage,
      Type exceptionType = Type::kSystem,
      const std::string& exceptionName = "SubstraitException");

  // Inherited
  [[nodiscard]] const char* what() const noexcept override {
    return msg_.c_str();
  }

 private:
  const std::string msg_;
};

class SubstraitUserError : public SubstraitException {
 public:
  SubstraitUserError(
      const char* file,
      size_t line,
      const char* function,
      const std::string& exceptionCode,
      const std::string& exceptionMessage,
      const std::string& exceptionName = "SubstraitUserError")
      : SubstraitException(
            file,
            line,
            function,
            exceptionCode,
            exceptionMessage,
            Type::kUser,
            exceptionName) {}
};

class SubstraitRuntimeError final : public SubstraitException {
 public:
  SubstraitRuntimeError(
      const char* file,
      size_t line,
      const char* function,
      const std::string& exceptionCode,
      const std::string& exceptionMessage,
      const std::string& exceptionName = "SubstraitRuntimeError")
      : SubstraitException(
            file,
            line,
            function,
            exceptionCode,
            exceptionMessage,
            Type::kSystem,
            exceptionName) {}
};

template <typename... Args>
std::string errorMessage(fmt::string_view fmt, const Args&... args) {
  return fmt::vformat(fmt, fmt::make_format_args(args...));
}

#define SUBSTRAIT_THROW(exception, errorCode, ...)                             \
  {                                                                            \
    auto message = ::io::substrait::common::errorMessage(__VA_ARGS__);         \
    throw exception(__FILE__, __LINE__, __FUNCTION__, errorCode, message);     \
  }

#define SUBSTRAIT_UNSUPPORTED(...)                                             \
  SUBSTRAIT_THROW(                                                             \
      ::io::substrait::common::SubstraitUserError,                             \
      ::io::substrait::common::error_code::kUnsupported,                       \
      ##__VA_ARGS__)

#define SUBSTRAIT_UNREACHABLE(...)                                             \
  SUBSTRAIT_THROW(                                                             \
      ::io::substrait::common::SubstraitRuntimeError,                          \
      ::io::substrait::common::error_code::kUnreachableCode,                   \
      ##__VA_ARGS__)

#define SUBSTRAIT_FAIL(...)                                                    \
  SUBSTRAIT_THROW(                                                             \
      ::io::substrait::common::SubstraitRuntimeError,                          \
      ::io::substrait::common::error_code::kInvalidState,                      \
      ##__VA_ARGS__)

#define SUBSTRAIT_USER_FAIL(...)                                               \
  SUBSTRAIT_THROW(                                                             \
      ::io::substrait::common::SubstraitUserError,                             \
      ::io::substrait::common::error_code::kInvalidState,                      \
      ##__VA_ARGS__)

#define SUBSTRAIT_NYI(...)                                                     \
  SUBSTRAIT_THROW(                                                             \
      ::io::substrait::common::SubstraitRuntimeError,                          \
      ::io::substrait::common::error_code::kNotImplemented,                    \
      ##__VA_ARGS__)

#define SUBSTRAIT_IVALID_ARGUMENT(...)                                         \
  SUBSTRAIT_THROW(                                                             \
      ::io::substrait::common::SubstraitUserError,                             \
      ::io::substrait::common::error_code::kInvalidArgument,                   \
      ##__VA_ARGS__)

} // namespace io::substrait::common
