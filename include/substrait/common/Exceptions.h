/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <memory>
#include <utility>
#include "fmt/format.h"

namespace io::substrait::common {
namespace error_code {

//====================== User Error Codes ======================:

// An error raised when an argument verification fails
inline constexpr auto kInvalidArgument = "INVALID_ARGUMENT";

// An error raised when a requested operation is not supported.
inline constexpr auto kUnsupported = "UNSUPPORTED";

//====================== Runtime Error Codes ======================:

// An error raised when the current state of a component is invalid.
inline constexpr auto kInvalidState = "INVALID_STATE";

// An error raised when unreachable code point was executed.
inline constexpr auto kUnreachableCode = "UNREACHABLE_CODE";

// An error raised when a requested operation is not yet supported.
inline constexpr auto kNotImplemented = "NOT_IMPLEMENTED";

// An error raised when a method has been passed an illegal or inappropriate
// argument.
inline constexpr auto kIllegalArgument = "ILLEGAL_ARGUMENT";

} // namespace error_code

class SubstraitException : public std::exception {
 public:
  enum class Type { kUser = 0, kSystem = 1 };

  SubstraitException(
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
      const std::string& exceptionCode,
      const std::string& exceptionMessage,
      const std::string& exceptionName = "SubstraitUserError")
      : SubstraitException(
            exceptionCode,
            exceptionMessage,
            Type::kUser,
            exceptionName) {}
};

class SubstraitRuntimeError final : public SubstraitException {
 public:
  SubstraitRuntimeError(
      const std::string& exceptionCode,
      const std::string& exceptionMessage,
      const std::string& exceptionName = "SubstraitRuntimeError")
      : SubstraitException(
            exceptionCode,
            exceptionMessage,
            Type::kSystem,
            exceptionName) {}
};

template <typename... Args>
std::string errorMessage(fmt::string_view fmt, const Args&... args) {
  return fmt::vformat(fmt, fmt::make_format_args(args...));
}

#define SUBSTRAIT_THROW(exception, errorCode, ...)                   \
  {                                                                  \
    auto message = io::substrait::common::errorMessage(__VA_ARGS__); \
    throw exception(errorCode, message);                             \
  }

#define SUBSTRAIT_UNSUPPORTED(...)                       \
  SUBSTRAIT_THROW(                                       \
      ::io::substrait::common::SubstraitUserError,       \
      ::io::substrait::common::error_code::kUnsupported, \
      ##__VA_ARGS__)

#define SUBSTRAIT_UNREACHABLE(...)                           \
  SUBSTRAIT_THROW(                                           \
      ::io::substrait::common::SubstraitRuntimeError,        \
      ::io::substrait::common::error_code::kUnreachableCode, \
      ##__VA_ARGS__)

#define SUBSTRAIT_FAIL(...)                               \
  SUBSTRAIT_THROW(                                        \
      ::io::substrait::common::SubstraitRuntimeError,     \
      ::io::substrait::common::error_code::kInvalidState, \
      ##__VA_ARGS__)

#define SUBSTRAIT_USER_FAIL(...)                          \
  SUBSTRAIT_THROW(                                        \
      ::io::substrait::common::SubstraitUserError,        \
      ::io::substrait::common::error_code::kInvalidState, \
      ##__VA_ARGS__)

#define SUBSTRAIT_NYI(...)                                  \
  SUBSTRAIT_THROW(                                          \
      ::io::substrait::common::SubstraitRuntimeError,       \
      ::io::substrait::common::error_code::kNotImplemented, \
      ##__VA_ARGS__)

#define SUBSTRAIT_ILLEGAL_ARGUMENT(...)                      \
  SUBSTRAIT_THROW(                                           \
      ::io::substrait::common::SubstraitUserError,           \
      ::io::substrait::common::error_code::kIllegalArgument, \
      ##__VA_ARGS__)

} // namespace io::substrait::common