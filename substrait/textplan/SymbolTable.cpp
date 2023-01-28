//
// Created by David Sisson on 1/25/23.
//

#include <map>
#include <string>
#include <vector>

#include "SymbolTable.h"

Location Location::visit(const std::string& name) const {
  Location new_location = *this;
  new_location.location_.push_back(name);
  return new_location;
}

std::string Location::toString() const {
  std::string text;
  for (const auto& loc : location_) {
    if (!text.empty())
      text += " -> ";
    text += loc;
  }
  return text;
}

bool SymbolTableIterator::operator!=(SymbolTableIterator const& other) const {
  return index_ != other.index_;
}

SymbolInfo const& SymbolTableIterator::operator*() const {
  return *table_->symbols_[index_];
}

SymbolTableIterator const& SymbolTableIterator::operator++() {
  ++index_;
  return *this;
}

std::string SymbolTable::getUniqueName(const std::string& base_name) {
  if (names_.find(base_name) == names_.end()) {
    names_.insert(std::make_pair(base_name, 1));
    return base_name;
  }
  names_[base_name]++;
  return base_name + std::to_string(names_[base_name]);
}

// MEGAHACK -- Is a symbol's location the combination of the name and location?
void SymbolTable::defineSymbol(
    const std::string& name,
    const Location& location,
    SymbolType type,
    const substrait::Rel::RelTypeCase& subtype,
    const void *blob) {
  // MEGAHACK -- Note that this does not detect attempts to reuse the same
  // symbol.
  std::shared_ptr<SymbolInfo> info =
      std::make_shared<SymbolInfo>(name, location, type, subtype, blob);
  symbols_.push_back(info);
  symbols_by_name_.insert(std::make_pair(name, symbols_.size()-1));
  symbols_by_location_.insert(std::make_pair(location.toString(), symbols_.size()-1));
}

void SymbolTable::defineUniqueSymbol(
    const std::string& name,
    const Location& location,
    SymbolType type,
    const substrait::Rel::RelTypeCase& subtype,
    const void* blob) {
  const std::string& unique_name = getUniqueName(name);
  defineSymbol(unique_name, location, type, subtype, blob);
}

std::shared_ptr<const SymbolInfo> SymbolTable::lookupSymbolByName(
    const std::string& name) {
  if (symbols_by_name_.find(name) != symbols_by_name_.end()) {
    return nullptr;
  }
  return symbols_[symbols_by_name_[name]];
}

std::shared_ptr<const SymbolInfo> SymbolTable::lookupSymbolByLocation(
    const Location& location) {
  const std::string& loc = location.toString();
  if (symbols_by_location_.find(loc) != symbols_by_location_.end()) {
    return nullptr;
  }
  return symbols_[symbols_by_location_[loc]];
}

std::shared_ptr<const SymbolInfo> SymbolTable::nthSymbolByType(uint32_t n, SymbolType type) {
  int count = 0;
  for (auto symbol : symbols_) {
    if (symbol->type == type) {
      if (n == count++) return symbol;
    }
  }
  return nullptr;
}

SymbolTableIterator SymbolTable::begin() {
  return {this, 0};
}

SymbolTableIterator SymbolTable::end() {
  return {this, symbols_by_name_.size()};
}
