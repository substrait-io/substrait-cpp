/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "substrait/textplan/Location.h"

namespace io::substrait::textplan {

enum class SymbolType {
  kExtensionSpace = 0,
  kFunction = 1,
  kPlanRelation = 2,
  kRelation = 3,
  kRelationDetail = 4,
  kSchema = 5,
  kSchemaColumn = 6,
  kSource = 7,
  kSourceDetail = 8,
  kField = 9,
  kRoot = 10,

  kUnknown = -1,
};

enum class RelationType {
  // Logical plans
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

  // Physical plans
  kHashJoin = 31,
  kMergeJoin = 32,

  // Write relations, currently unreachable in Plan protos.
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
  Location location;
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
        location(newLocation),
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

  // Changes the location for a specified existing symbol.
  void updateLocation(const SymbolInfo& symbol, const Location& location);

  [[nodiscard]] const SymbolInfo* lookupSymbolByName(
      const std::string& name) const;

  [[nodiscard]] const SymbolInfo* lookupSymbolByLocation(
      const Location& location) const;

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

 private:
  // Returns the table size if the symbol is not found.
  size_t findSymbolIndex(const SymbolInfo& symbol);

  friend SymbolTableIterator;

  std::unordered_map<std::string, int32_t> names_;

  std::vector<std::shared_ptr<SymbolInfo>> symbols_;
  std::unordered_map<std::string, size_t> symbolsByName_;
  std::unordered_map<Location, size_t> symbolsByLocation_;
};

} // namespace io::substrait::textplan
