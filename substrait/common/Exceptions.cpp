/* SPDX-License-Identifier: Apache-2.0 */

#include <fmt/format.h>
#include "substrait/common/Exceptions.h"

namespace io::substrait::common {

SubstraitException::SubstraitException(
    const std::string& exceptionCode,
    const std::string& exceptionMessage,
    Type exceptionType,
    const std::string& exceptionName)
    : msg_(fmt::format(
          "Exception: {}\nError Code: {}\nType: {}\nReason: {}\n"
          "Function: {}\nFile: {}\n:Line: {}\n",
          exceptionName,
          exceptionCode,
          exceptionType == Type::kSystem ? "system" : "user",
          exceptionMessage,
          __FUNCTION__,
          __FILE__,
          std::to_string(__LINE__))) {}

} // namespace io::substrait::common
