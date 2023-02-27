#include <any>
#include <map>
#include <string>

#include "SymbolTable.h"

#include "substrait/textplan/Location.h"

namespace io::substrait::textplan {

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
  auto itr = names_.find(base_name);
  if (itr == names_.end()) {
    names_.insert(std::make_pair(base_name, 1));
    return base_name;
  }
  int32_t count = itr->second + 1;
  itr->second = count;
  return base_name + std::to_string(count);
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
  symbols_.push_back(info);
  symbols_by_name_.insert(std::make_pair(name, symbols_.size() - 1));
  symbols_by_location_.insert(std::make_pair(location, symbols_.size() - 1));

  return info.get();
}

SymbolInfo* SymbolTable::defineUniqueSymbol(
    const std::string& name,
    const Location& location,
    SymbolType type,
    const std::any& subtype,
    const std::any& blob) {
  const std::string& unique_name = getUniqueName(name);
  return defineSymbol(unique_name, location, type, subtype, blob);
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
  if (symbols_by_location_.find(location) != symbols_by_location_.end()) {
    return nullptr;
  }
  return symbols_[symbols_by_location_[location]];
}

std::shared_ptr<const SymbolInfo> SymbolTable::nthSymbolByType(
    uint32_t n,
    SymbolType type) {
  int count = 0;
  for (auto symbol : symbols_) {
    if (symbol->type == type) {
      if (n == count++)
        return symbol;
    }
  }
  return nullptr;
}

SymbolTableIterator SymbolTable::begin() const {
  return {this, 0};
}

SymbolTableIterator SymbolTable::end() const {
  return {this, symbols_by_name_.size()};
}

} // namespace io::substrait::textplan