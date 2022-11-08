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

#include "substrait/common/Exceptions.h"
#include <fmt/format.h>

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
