// MEGAHACK -- Fix header preamble on all new files (perhaps fix CLion default as well?).

#ifndef SUBSTRAIT_CPP_SYMBOLTABLE_H
#define SUBSTRAIT_CPP_SYMBOLTABLE_H

#include <string>
#include <map>
#include <vector>

class Location {
 public:
  [[nodiscard]] Location visit(const std::string& name) const;

  [[nodiscard]] std::string toString() const;

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

class SymbolTable;

class SymbolTableIterator {
 public:
  bool operator!= (SymbolTableIterator const & other) const;

  SymbolInfo const & operator* () const;

  SymbolTableIterator const & operator++ ();

 private:
  friend SymbolTable;

  SymbolTableIterator(SymbolTable* table, size_t start) {
    table_ = table;
    index_ = start;
  }

  size_t index_;
  SymbolTable* table_;
};

class SymbolTable {
 public:
  std::string getUniqueName(const std::string& base_name);

  // MEGAHACK -- Is a symbol's location the combination of the name and location?
  void defineSymbol(const std::string& name, const Location& location, SymbolType type);

  void defineUniqueSymbol(const std::string& name, const Location& location, SymbolType type);

  std::shared_ptr<const SymbolInfo> lookupSymbolByName(const std::string& name);

  std::shared_ptr<const SymbolInfo> lookupSymbolByLocation(const Location& location);

  SymbolTableIterator begin();

  SymbolTableIterator end();

 private:
  friend SymbolTableIterator;

  std::map<std::string, int32_t> names_;

  std::vector<std::shared_ptr<SymbolInfo>> symbols_;
  std::map<std::string, size_t> symbols_by_name_;
  std::map<std::string, size_t> symbols_by_location_;
};

#endif // SUBSTRAIT_CPP_SYMBOLTABLE_H
