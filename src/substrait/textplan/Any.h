/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>

#include "fmt/format.h"

namespace io::substrait::textplan {

template <class ValueType>
inline ValueType any_cast(const std::any& value, const char* file, int line) {
  try {
    return std::any_cast<ValueType>(value);
  } catch (std::bad_any_cast& ex) {
    throw std::invalid_argument(
        fmt::format("{}:{} - {}", file, line, "bad any cast"));
  }
}

// A wrapper around std::any_cast that provides exceptions with line numbers.
#define ANY_CAST(ValueType, Value) \
  ::io::substrait::textplan::any_cast<ValueType>(Value, __FILE__, __LINE__)

} // namespace io::substrait::textplan
