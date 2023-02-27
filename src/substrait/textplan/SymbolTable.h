/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>
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

  kUnknown = -1,
};

enum class RelationType {
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
  kExchange = 10,
  kDdl = 11,
  kWrite = 12,
  kHashJoin = 13,
  kMergeJoin = 14,
  kReference = 15,

  kExtensionLeaf = 100,
  kExtensionSingle = 101,
  kExtensionMulti = 102,
};

enum class RelationDetailType {
  kUnknown = 0,
  kExpression = 1,
};

struct SymbolInfo {
  std::string name;
  Location location;
  SymbolType type;
  std::any relation_type;
  std::any blob;

  SymbolInfo(
      std::string new_name,
      Location new_location,
      SymbolType new_type,
      std::any new_relation_type,
      std::any new_blob)
      : name(std::move(new_name)),
        location(std::move(new_location)),
        type(new_type),
        relation_type(std::move(new_relation_type)),
        blob(std::move(new_blob)){};
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
  std::string getUniqueName(const std::string& base_name);

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

  std::shared_ptr<const SymbolInfo> lookupSymbolByName(const std::string& name);

  std::shared_ptr<const SymbolInfo> lookupSymbolByLocation(
      const Location& location);

  std::shared_ptr<const SymbolInfo> nthSymbolByType(
      uint32_t n,
      SymbolType type);

  [[nodiscard]] SymbolTableIterator begin() const;

  [[nodiscard]] SymbolTableIterator end() const;

  [[nodiscard]] const std::vector<std::shared_ptr<SymbolInfo>>& getSymbols()
      const {
    return symbols_;
  };

  // Temporary functions to allow externally computed text to be saved.
  void addCachedOutput(const std::string& text) {
    cached_output_ = text;
  }
  // TODO: Remove after we have the information required to reconstruct the
  // plan.
  [[nodiscard]] std::string getCachedOutput() const {
    return cached_output_;
  }

  // Add the capability for ::testing::PrintToString to print this.
  friend std::ostream& operator<<(std::ostream& os, const SymbolTable& result) {
    os << std::string("{");
    bool outputFirst = false;
    for (const auto& symbol : result.getSymbols()) {
      if (outputFirst) {
        os << std::string(", ");
      }
      os << symbol->name;
      outputFirst = true;
    }
    os << std::string("}");
    return os;
  }

 private:
  friend SymbolTableIterator;

  std::map<std::string, int32_t> names_;

  std::vector<std::shared_ptr<SymbolInfo>> symbols_;
  std::map<std::string, size_t> symbols_by_name_;
  std::map<Location, size_t> symbols_by_location_;

  std::string cached_output_;
};

} // namespace io::substrait::textplan