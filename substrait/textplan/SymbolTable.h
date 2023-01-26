// MEGAHACK -- Fix header preamble on all new files (perhaps fix CLion default as well?).

#ifndef SUBSTRAIT_CPP_SYMBOLTABLE_H
#define SUBSTRAIT_CPP_SYMBOLTABLE_H

#include <string>
#include <map>
#include <vector>

class Location {
 public:
  [[nodiscard]] Location visit(const std::string& name) const {
    Location new_location = *this;
    new_location.location_.push_back(name);
    return new_location;
  }

  std::string toString() {
    std::string text;
    for (const auto& loc : location_) {
      if (!text.empty()) text += " -> ";
      text += loc;
    }
    return text;
  }

 private:
  std::vector<std::string> location_;
};

// MEGAHACK -- Check style guide for enums.
enum SymbolType {
  kFunction = 0,
  kRelation = 1,
  kSchema = 2,
  kUnknown = -1,
};

struct SymbolInfo {
  std::string name;
  Location location;
  SymbolType type;

  SymbolInfo(const std::string& new_name, const Location& new_location, SymbolType new_type) {
    name = new_name;
    location = new_location;
    type = new_type;
  }
};

class SymbolTable {
 public:
  std::string getUniqueName(const std::string& base_name) {
    if (names_.find(base_name) == names_.end()) {
      names_.insert(std::make_pair(base_name, 1));
      return base_name;
    }
    names_[base_name]++;
    return base_name + std::to_string(names_[base_name]);
  }

  // MEGAHACK -- Is a symbol's location the combination of the name and location?
  void defineSymbol(const std::string& name, Location location, SymbolType type) {
    // MEGAHACK -- Note that this does not detect attempts to reuse the same symbol.
    std::shared_ptr<SymbolInfo> info = std::make_shared<SymbolInfo>(name, location, type);
    symbols_by_name_.insert(std::make_pair(name, info));
    symbols_by_location_.insert(std::make_pair(location.toString(), info));
  }

  void defineUniqueSymbol(const std::string& name, Location location, SymbolType type) {
    const std::string& unique_name = getUniqueName(name);
    defineSymbol(unique_name, location, type);
  }

  std::shared_ptr<const SymbolInfo> lookupSymbolByName(const std::string& name) {
    if (symbols_by_name_.find(name) != symbols_by_name_.end()) {
      return nullptr;
    }
    return symbols_by_name_[name];
  }

  std::shared_ptr<const SymbolInfo> lookupSymbolByLocation(Location location) {
    const std::string& loc = location.toString();
    if (symbols_by_location_.find(loc) != symbols_by_location_.end()) {
      return nullptr;
    }
    return symbols_by_location_[loc];
  }

 private:
  std::map<std::string, int32_t> names_;

  std::map<std::string, std::shared_ptr<SymbolInfo>> symbols_by_name_;
  std::map<std::string, std::shared_ptr<SymbolInfo>> symbols_by_location_;
};

#endif // SUBSTRAIT_CPP_SYMBOLTABLE_H
