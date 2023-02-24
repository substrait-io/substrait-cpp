/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <variant>
#include <vector>

namespace antlr4 {
class ParserRuleContext;
}

namespace io::substrait::textplan {

class ProtoLocation {
 public:
  [[nodiscard]] ProtoLocation visit(const std::string& name) const;

  [[nodiscard]] std::string to_string() const;

 private:
  std::vector<std::string> location_;
};

class Location {
 public:
  explicit Location(antlr4::ParserRuleContext* node) {
    loc_ = node;
  };
  explicit Location(ProtoLocation location) {
    loc_ = location;
  };

 private:
  friend std::hash<Location>;
  friend std::less<Location>;

  std::variant<ProtoLocation, antlr4::ParserRuleContext*> loc_;
};

} // namespace io::substrait::textplan

template <>
struct std::hash<::io::substrait::textplan::Location> {
  std::size_t operator()(const ::io::substrait::textplan::Location& loc) const noexcept;
};

template <>
struct std::less<::io::substrait::textplan::Location> {
  std::size_t operator()(
      const ::io::substrait::textplan::Location& lhs,
      const ::io::substrait::textplan::Location& rhs) const noexcept;
};
