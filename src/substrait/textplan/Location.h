/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <variant>
#include <vector>

namespace antlr4 {
class ParserRuleContext;
}

namespace google::protobuf {
class Message;
};

namespace io::substrait::textplan {

// Location is used for keeping track of where a symbol is within a parse tree.
// Since SymbolTable supports both antlr4 and protobuf messages there are
// essentially two flavors of location.  It is expected that only one type of
// location would be used in any SymbolTable instance.
class Location {
 public:
  constexpr explicit Location(antlr4::ParserRuleContext* node) : loc_(node) {}

  constexpr explicit Location(google::protobuf::Message* msg) : loc_(msg) {}

  static const Location kUnknownLocation;

 protected:
  friend bool operator==(const Location& c1, const Location& c2);

 private:
  friend std::hash<Location>;
  friend std::less<Location>;

  std::variant<antlr4::ParserRuleContext*, google::protobuf::Message*> loc_;
};

} // namespace io::substrait::textplan

template <>
struct std::hash<::io::substrait::textplan::Location> {
  std::size_t operator()(
      const ::io::substrait::textplan::Location& loc) const noexcept;
};

template <>
struct std::less<::io::substrait::textplan::Location> {
  bool operator()(
      const ::io::substrait::textplan::Location& lhs,
      const ::io::substrait::textplan::Location& rhs) const noexcept;
};

// Convenience macro useful for construction protobuffer based locations.
#define PROTO_LOCATION(proto)                                                  \
  ::io::substrait::textplan::Location((::google::protobuf::Message*)&proto)
