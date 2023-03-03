/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/common/Exceptions.h"

#include <fmt/format.h>

namespace io::substrait::common {

SubstraitException::SubstraitException(
    const char* file,
    size_t line,
    const char* function,
    const std::string& exceptionCode,
    const std::string& exceptionMessage,
    Type exceptionType,
    const std::string& exceptionName)
    : msg_(fmt::format(
          "Exception: {}\nError Code: {}\nError Type: {}\nReason: {}\n"
          "Function: {}\nLocation: {}(Line:{})\n",
          exceptionName,
          exceptionCode,
          exceptionType == Type::kSystem ? "system" : "user",
          exceptionMessage,
          function,
          file,
          std::to_string(line))) {}

} // namespace io::substrait::common
