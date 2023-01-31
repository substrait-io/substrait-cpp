// MEGAHACK -- Fix header preamble on all new files (perhaps fix CLion default
// as well?).

#ifndef SUBSTRAIT_CPP_SYMBOLTABLE_H
#define SUBSTRAIT_CPP_SYMBOLTABLE_H

#include <any>
#include <map>
#include <string>
#include <vector>

#include "substrait/algebra.pb.h"

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
  kField = 1,
  kPlanRelation = 2, // MEGAHACK -- May not be interesting.
  kRelation = 3,
  kSchema = 4,
  kSource = 5,
  kExtensionSpace = 6,
  kUnknown = -1,
};

struct SymbolInfo {
  std::string name;
  Location location;
  SymbolType type;
  substrait::Rel::RelTypeCase relation_type;
  std::any blob;

  SymbolInfo(
      const std::string& new_name,
      const Location& new_location,
      SymbolType new_type,
      const substrait::Rel::RelTypeCase& new_relation_type,
      const std::any& new_blob) {
    name = new_name;
    location = new_location;
    type = new_type;
    relation_type = new_relation_type;
    blob = new_blob;
  }
};

class SymbolTable;

class SymbolTableIterator {
 public:
  bool operator!=(SymbolTableIterator const& other) const;

  SymbolInfo const& operator*() const;

  SymbolTableIterator const& operator++();

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

  // MEGAHACK -- Is a symbol's location the combination of the name and
  // location?
  void defineSymbol(
      const std::string& name,
      const Location& location,
      SymbolType type,
      const substrait::Rel::RelTypeCase& subtype,
      const std::any& blob);

  void defineUniqueSymbol(
      const std::string& name,
      const Location& location,
      SymbolType type,
      const substrait::Rel::RelTypeCase& subtype,
      const std::any& blob);

  std::shared_ptr<const SymbolInfo> lookupSymbolByName(const std::string& name);

  std::shared_ptr<const SymbolInfo> lookupSymbolByLocation(
      const Location& location);

  std::shared_ptr<const SymbolInfo> nthSymbolByType(
      uint32_t n,
      SymbolType type);

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
