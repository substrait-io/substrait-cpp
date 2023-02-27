/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/Location.h"

#include <functional>
#include <sstream>
#include <string>

namespace io::substrait::textplan {

ProtoLocation ProtoLocation::visit(const std::string& name) const {
  ProtoLocation new_location = *this;
  new_location.location_.push_back(name);
  return new_location;
}

std::string ProtoLocation::toString() const {
  std::stringstream text;
  bool written = false;
  for (const auto& loc : location_) {
    if (!written) {
      text << " -> ";
      written = true;
    }
    text << loc;
  }
  return text.str();
}

} // namespace io::substrait::textplan

std::size_t std::hash<::io::substrait::textplan::Location>::operator()(
    const ::io::substrait::textplan::Location& loc) const noexcept {
  if (std::holds_alternative<::io::substrait::textplan::ProtoLocation>(
          loc.loc_)) {
    return std::hash<std::string>()(
        std::get<::io::substrait::textplan::ProtoLocation>(loc.loc_)
            .toString());
  } else if (std::holds_alternative<antlr4::ParserRuleContext*>(loc.loc_)) {
    return std::hash<antlr4::ParserRuleContext*>()(
        std::get<antlr4::ParserRuleContext*>(loc.loc_));
  }
  // Should not be reached.
  return 0;
}

std::size_t std::less<::io::substrait::textplan::Location>::operator()(
    const ::io::substrait::textplan::Location& lhs,
    const ::io::substrait::textplan::Location& rhs) const noexcept {
  if (std::holds_alternative<::io::substrait::textplan::ProtoLocation>(
          lhs.loc_)) {
    return std::get<::io::substrait::textplan::ProtoLocation>(lhs.loc_)
               .toString() <
        std::get<::io::substrait::textplan::ProtoLocation>(rhs.loc_).toString();
  }
  return std::get<antlr4::ParserRuleContext*>(lhs.loc_) <
      std::get<antlr4::ParserRuleContext*>(rhs.loc_);
}
