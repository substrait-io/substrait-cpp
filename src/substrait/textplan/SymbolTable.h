/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "substrait/textplan/Location.h"

namespace io::substrait::textplan {

enum class SymbolType {
  kExtensionSpace = 0,
  kFunction = 1,
  kPlanRelation = 2,
  kRelation = 3,
  kSchema = 5,
  kSchemaColumn = 6,
  kSource = 7,
  kSourceDetail = 8,
  kField = 9,
  kRoot = 10,
  kTable = 11,
  kMeasure = 12,

  kUnknown = -1,
};

enum class RelationType {
  // Logical
  kUnknown = 0,
  kRead = 1,
  kProject = 2,
  kJoin = 3,
  kCross = 4,
  kFetch = 5,
  kAggregate = 6,
  kSort = 7,
  kFilter = 8,
  kSet = 9,

  // Physical
  kHashJoin = 31,
  kMergeJoin = 32,

  // Write
  kExchange = 50,
  kDdl = 51,
  kWrite = 52,

  // Extensions
  kExtensionLeaf = 100,
  kExtensionSingle = 101,
  kExtensionMulti = 102,
};

enum class RelationDetailType {
  kUnknown = 0,
  kExpression = 1,
};

enum class SourceType {
  kUnknown = 0,
  kLocalFiles = 1,
  kNamedTable = 2,
  kVirtualTable = 3,
  kExtensionTable = 4,
};

const std::string& symbolTypeName(SymbolType type);

struct SymbolInfo {
  std::string name;
  std::string alias{}; // If present, use this instead of name.
  const SymbolInfo* schema{nullptr}; // The related schema symbol if present.
  Location sourceLocation;
  Location permanentLocation{Location::kUnknownLocation};
  Location parentQueryLocation{Location::kUnknownLocation};
  int parentQueryIndex{-1};
  SymbolType type;
  std::any subtype;
  std::any blob;

  SymbolInfo(
      std::string newName,
      Location newLocation,
      SymbolType newType,
      std::any newSubtype,
      std::any newBlob)
      : name(std::move(newName)),
        sourceLocation(newLocation),
        type(newType),
        subtype(std::move(newSubtype)),
        blob(std::move(newBlob)){};

  static const SymbolInfo kUnknown;

  friend bool operator==(const SymbolInfo& left, const SymbolInfo& right);
  friend bool operator!=(const SymbolInfo& left, const SymbolInfo& right);
};

class SymbolTable;

class SymbolTableIterator {
 public:
  bool operator!=(SymbolTableIterator const& other) const;

  const SymbolInfo& operator*() const;

  SymbolTableIterator operator++();

 private:
  friend SymbolTable;

  SymbolTableIterator(const SymbolTable* table, size_t start)
      : table_(table), index_(start){};

  size_t index_;
  const SymbolTable* table_;
};

class SymbolTable {
 public:
  // If the given symbol is not yet defined, returns that symbol.  Otherwise
  // it returns a modified version of the symbol (by adding a number) so that
  // it is unique.
  std::string getUniqueName(const std::string& baseName);

  // Adds a new symbol to the symbol table.
  SymbolInfo* defineSymbol(
      const std::string& name,
      const Location& location,
      SymbolType type,
      const std::any& subtype,
      const std::any& blob);

  // Convenience function that defines a symbol by first calling getUniqueName.
  SymbolInfo* defineUniqueSymbol(
      const std::string& name,
      const Location& location,
      SymbolType type,
      const std::any& subtype,
      const std::any& blob);

  // Changes the permanent location for a specified existing symbol.
  void addPermanentLocation(const SymbolInfo& symbol, const Location& location);

  // Sets the location of the parent query.
  void setParentQueryLocation(
      const SymbolInfo& symbol,
      const Location& location);

  // Adds an alias to the given symbol.
  void addAlias(const std::string& alias, const SymbolInfo* symbol);

  [[nodiscard]] const SymbolInfo* lookupSymbolByName(
      const std::string& name) const;

  [[nodiscard]] std::vector<const SymbolInfo*> lookupSymbolsByLocation(
      const Location& location) const;

  [[nodiscard]] const SymbolInfo* lookupSymbolByLocationAndType(
      const Location& location,
      SymbolType type) const;

  [[nodiscard]] const SymbolInfo* lookupSymbolByLocationAndTypes(
      const Location& location,
      std::unordered_set<SymbolType> types) const;

  [[nodiscard]] const SymbolInfo* lookupSymbolByParentQueryAndType(
      const Location& location,
      int index,
      SymbolType type) const;

  [[nodiscard]] const SymbolInfo& nthSymbolByType(uint32_t n, SymbolType type)
      const;

  [[nodiscard]] SymbolTableIterator begin() const;

  [[nodiscard]] SymbolTableIterator end() const;

  [[nodiscard]] const std::vector<std::shared_ptr<SymbolInfo>>& getSymbols()
      const {
    return symbols_;
  };

  // Add the capability for ::testing::PrintToString to print this.
  friend std::ostream& operator<<(std::ostream& os, const SymbolTable& result) {
    os << std::string("{");
    bool hasPreviousText = false;
    for (const auto& symbol : result.getSymbols()) {
      if (hasPreviousText) {
        os << std::string(", ");
      }
      os << symbol->name;
      hasPreviousText = true;
    }
    os << std::string("}");
    return os;
  }

  [[nodiscard]] std::string toDebugString() const;

 private:
  // Returns the table size if the symbol is not found.
  size_t findSymbolIndex(const SymbolInfo& symbol);

  friend SymbolTableIterator;

  std::unordered_map<std::string, int32_t> names_;

  std::vector<std::shared_ptr<SymbolInfo>> symbols_;
  std::unordered_map<std::string, size_t> symbolsByName_;
  std::multimap<Location, size_t> symbolsByLocation_;
};

} // namespace io::substrait::textplan
