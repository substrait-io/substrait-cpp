/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/Location.h"

#include <functional>

namespace io::substrait::textplan {

constexpr Location Location::kUnknownLocation(
    static_cast<google::protobuf::Message*>(nullptr));

bool operator==(const Location& c1, const Location& c2) {
  // Test only one side since we only store one kind of content per table.
  if (std::holds_alternative<antlr4::ParserRuleContext*>(c1.loc_)) {
    if (!std::holds_alternative<antlr4::ParserRuleContext*>(c2.loc_)) {
      return false;
    }
    auto a1 = std::get<antlr4::ParserRuleContext*>(c1.loc_);
    auto a2 = std::get<antlr4::ParserRuleContext*>(c2.loc_);
    return a1 == a2;
  } else if (std::holds_alternative<google::protobuf::Message*>(c1.loc_)) {
    if (!std::holds_alternative<google::protobuf::Message*>(c2.loc_)) {
      return false;
    }
    auto a1 = std::get<google::protobuf::Message*>(c1.loc_);
    auto a2 = std::get<google::protobuf::Message*>(c2.loc_);
    return a1 == a2;
  }
  // Should not be reached.
  return false;
}

} // namespace io::substrait::textplan

std::size_t std::hash<::io::substrait::textplan::Location>::operator()(
    const ::io::substrait::textplan::Location& loc) const noexcept {
  if (std::holds_alternative<antlr4::ParserRuleContext*>(loc.loc_)) {
    return std::hash<antlr4::ParserRuleContext*>()(
        std::get<antlr4::ParserRuleContext*>(loc.loc_));
  } else if (std::holds_alternative<google::protobuf::Message*>(loc.loc_)) {
    return std::hash<google::protobuf::Message*>()(
        std::get<google::protobuf::Message*>(loc.loc_));
  }
  // Should not be reached.
  return 0;
}

bool std::less<::io::substrait::textplan::Location>::operator()(
    const ::io::substrait::textplan::Location& lhs,
    const ::io::substrait::textplan::Location& rhs) const noexcept {
  if (std::holds_alternative<antlr4::ParserRuleContext*>(lhs.loc_)) {
    if (!std::holds_alternative<antlr4::ParserRuleContext*>(rhs.loc_)) {
      // This alternative is always less than the remaining choices.
      return true;
    }
    return std::get<antlr4::ParserRuleContext*>(lhs.loc_) <
        std::get<antlr4::ParserRuleContext*>(rhs.loc_);
  } else if (std::holds_alternative<google::protobuf::Message*>(lhs.loc_)) {
    if (!std::holds_alternative<google::protobuf::Message*>(rhs.loc_)) {
      // This alternative is always less than the remaining (zero) choices.
      return true;
    }
    return std::get<google::protobuf::Message*>(lhs.loc_) <
        std::get<google::protobuf::Message*>(rhs.loc_);
  }
  // Should not be reached.
  return false;
}
