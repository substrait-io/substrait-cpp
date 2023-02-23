/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>

#include "fmt/format.h"

namespace io::substrait::textplan {

template <typename ValueType>
inline const ValueType*
any_cast(const std::any* value, const char* file, int line) noexcept {
  try {
    auto result = std::any_cast<ValueType>(value);
    if (result == nullptr) {
      throw std::invalid_argument(
        fmt::format("{}:{} - {}", file, line, "bad any cast"));
    }
    return result;
  } catch (std::bad_any_cast& ex) {
    throw std::invalid_argument(
        fmt::format("{}:{} - {}", file, line, "bad any cast"));
  }
}

template <typename ValueType>
inline ValueType*
any_cast(std::any* value, const char* file, int line) noexcept {
  try {
    auto result = std::any_cast<ValueType>(value);
    if (result == nullptr) {
      throw std::invalid_argument(
          fmt::format("{}:{} - {}", file, line, "bad any cast"));
    }
    return result;
  } catch (std::bad_any_cast& ex) {
    throw std::invalid_argument(
        fmt::format("{}:{} - {}", file, line, "bad any cast"));
  }
}

template <class ValueType>
inline ValueType any_cast(const std::any& value, const char* file, int line) {
  try {
    return std::any_cast<ValueType>(value);
  } catch (std::bad_any_cast& ex) {
    throw std::invalid_argument(
        fmt::format("{}:{} - {}", file, line, "bad any cast"));
  }
}

template <class ValueType>
inline ValueType any_cast(std::any& value, const char* file, int line) {
  try {
    return std::any_cast<ValueType>(value);
  } catch (std::bad_any_cast& ex) {
    throw std::invalid_argument(
        fmt::format("{}:{} - {}", file, line, "bad any cast"));
  }
}

#define ANY_CAST(ValueType, Value) \
  ::io::substrait::textplan::any_cast<ValueType>(Value, __FILE__, __LINE__)

} // namespace io::substrait::textplan