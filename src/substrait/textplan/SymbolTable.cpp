/* SPDX-License-Identifier: Apache-2.0 */
#include "SymbolTable.h"

#include <any>
#include <map>
#include <string>

#include "SymbolTable.h"

#include "substrait/common/Exceptions.h"
#include "substrait/textplan/Location.h"

namespace io::substrait::textplan {

const std::string& SymbolTypeName(SymbolType type) {
  static std::vector<std::string> names = {
      "kExtensionSpace",
      "kFunction",
      "kPlanRelation",
      "kRelation",
      "kRelationDetail",
      "kSchema",
      "kSchemaColumn",
      "kSource",
      "kSourceDetail",
      "kField",
  };
  auto typeNum = int32_t(type);
  if (typeNum == -1) {
    static std::string unknown = "kUnknown";
    return unknown;
  }
  if (typeNum > names.size()) {
    SUBSTRAIT_FAIL(
        "Unknown symbol type " + std::to_string(typeNum) + " referenced.");
  }

  return names[typeNum];
}

bool SymbolTableIterator::operator!=(SymbolTableIterator const& other) const {
  return index_ != other.index_;
}

SymbolInfo const& SymbolTableIterator::operator*() const {
  return *table_->symbols_[index_];
}

SymbolTableIterator SymbolTableIterator::operator++() {
  ++index_;
  return *this;
}

const SymbolInfo SymbolInfo::kUnknown = {
    "__UNKNOWN__",
    Location::kUnknownLocation,
    SymbolType::kUnknown,
    std::nullopt,
    std::nullopt};

bool operator==(const SymbolInfo& left, const SymbolInfo& right) {
  return (left.name == right.name) && (left.location == right.location) &&
      (left.type == right.type);
}

bool operator!=(const SymbolInfo& left, const SymbolInfo& right) {
  return !(left == right);
}

std::string SymbolTable::getUniqueName(const std::string& baseName) {
  auto symbolSeenCount = names_.find(baseName);
  if (symbolSeenCount == names_.end()) {
    names_.insert(std::make_pair(baseName, 1));
    return baseName;
  }
  int32_t count = symbolSeenCount->second + 1;
  symbolSeenCount->second = count;
  return baseName + std::to_string(count);
}

SymbolInfo* SymbolTable::defineSymbol(
    const std::string& name,
    const Location& location,
    SymbolType type,
    const std::any& subtype,
    const std::any& blob) {
  // TODO -- Detect attempts to reuse the same symbol.
  std::shared_ptr<SymbolInfo> info =
      std::make_shared<SymbolInfo>(name, location, type, subtype, blob);
  symbols_.push_back(std::move(info));
  symbolsByName_.insert(std::make_pair(name, symbols_.size() - 1));
  symbolsByLocation_.insert(std::make_pair(location, symbols_.size() - 1));

  return info.get();
}

SymbolInfo* SymbolTable::defineUniqueSymbol(
    const std::string& name,
    const Location& location,
    SymbolType type,
    const std::any& subtype,
    const std::any& blob) {
  std::string uniqueName = getUniqueName(name);
  return defineSymbol(uniqueName, location, type, subtype, blob);
}

const SymbolInfo& SymbolTable::lookupSymbolByName(const std::string& name) {
  auto itr = symbolsByName_.find(name);
  if (itr == symbolsByName_.end()) {
    return SymbolInfo::kUnknown;
  }
  return *symbols_[itr->second];
}

const SymbolInfo& SymbolTable::lookupSymbolByLocation(
    const Location& location) {
  auto itr = symbolsByLocation_.find(location);
  if (itr == symbolsByLocation_.end()) {
    return SymbolInfo::kUnknown;
  }
  return *symbols_[itr->second];
}

const SymbolInfo& SymbolTable::nthSymbolByType(uint32_t n, SymbolType type) {
  int count = 0;
  for (const auto& symbol : symbols_) {
    if (symbol->type == type) {
      if (n == count++) {
        return *symbol;
      }
    }
  }
  return SymbolInfo::kUnknown;
}

SymbolTableIterator SymbolTable::begin() const {
  return {this, 0};
}

SymbolTableIterator SymbolTable::end() const {
  return {this, symbolsByName_.size()};
}

} // namespace io::substrait::textplan
