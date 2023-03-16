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

SymbolTableIterator SymbolTableIterator::operator++() {
  ++index_;
  return *this;
}

bool operator==(const SymbolInfo& left, const SymbolInfo& right) {
  return (left.name == right.name) && (left.location == right.location) &&
      (left.type == right.type);
}

bool operator!=(const SymbolInfo& left, const SymbolInfo& right) {
  return !(left == right);
}

std::string SymbolTable::getUniqueName(const std::string& base_name) {
  auto symbolSeenCount = names_.find(base_name);
  if (symbolSeenCount == names_.end()) {
    names_.insert(std::make_pair(base_name, 1));
    return base_name;
  }
  int32_t count = symbolSeenCount->second + 1;
  symbolSeenCount->second = count;
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
  symbols_.push_back(std::move(info));
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
  std::string unique_name = getUniqueName(name);
  return defineSymbol(unique_name, location, type, subtype, blob);
}

const SymbolInfo& SymbolTable::lookupSymbolByName(const std::string& name) {
  auto itr = symbols_by_name_.find(name);
  if (itr == symbols_by_name_.end()) {
    return kUnknownSymbol;
  }
  return *symbols_[itr->second];
}

const SymbolInfo& SymbolTable::lookupSymbolByLocation(
    const Location& location) {
  auto itr = symbols_by_location_.find(location);
  if (itr == symbols_by_location_.end()) {
    return kUnknownSymbol;
  }
  return *symbols_[itr->second];
}

const SymbolInfo& SymbolTable::nthSymbolByType(uint32_t n, SymbolType type) {
  int count = 0;
  for (const auto& symbol : symbols_) {
    if (symbol->type == type) {
      if (n == count++)
        return *symbol;
    }
  }
  return kUnknownSymbol;
}

SymbolTableIterator SymbolTable::begin() const {
  return {this, 0};
}

SymbolTableIterator SymbolTable::end() const {
  return {this, symbols_by_name_.size()};
}

const SymbolInfo SymbolTable::kUnknownSymbol = {
    "__UNKNOWN__",
    Location::kUnknownLocation,
    SymbolType::kUnknown,
    std::nullopt,
    std::nullopt};

} // namespace io::substrait::textplan
