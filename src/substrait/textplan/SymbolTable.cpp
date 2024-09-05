/* SPDX-License-Identifier: Apache-2.0 */
#include "substrait/textplan/SymbolTable.h"

#include <any>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>

#include "substrait/common/Exceptions.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/Location.h"
#include "substrait/textplan/StructuredSymbolData.h"

using namespace io::substrait::textplan;

const std::string& symbolTypeName(SymbolType type) {
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

bool io::substrait::textplan::operator==(
    const SymbolInfo& left,
    const SymbolInfo& right) {
  return (left.name == right.name) &&
      (left.sourceLocation == right.sourceLocation) &&
      (left.type == right.type);
}

bool io::substrait::textplan::operator!=(
    const SymbolInfo& left,
    const SymbolInfo& right) {
  return !(left == right);
}

std::string SymbolTable::getUniqueName(const std::string& baseName) {
  auto symbolSeenCount = names_.find(baseName);
  if (symbolSeenCount == names_.end()) {
    names_.insert(std::make_pair(baseName, 1));
    return baseName;
  }
  const int32_t count = symbolSeenCount->second + 1;
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

  return symbols_.back().get();
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

size_t SymbolTable::findSymbolIndex(const SymbolInfo& symbol) {
  if (symbols_.empty()) {
    return symbols_.size();
  }
  for (int index = 0; index < symbols_.size(); ++index) {
    if (*symbols_[index] == symbol) {
      return index;
    }
  }
  return symbols_.size();
}

void SymbolTable::addPermanentLocation(
    const SymbolInfo& symbol,
    const Location& location) {
  auto index = findSymbolIndex(symbol);
  symbols_[index]->permanentLocation = location;
  symbolsByLocation_.insert(std::make_pair(location, index));
}

void SymbolTable::setParentQueryLocation(
    const io::substrait::textplan::SymbolInfo& symbol,
    const io::substrait::textplan::Location& location) {
  auto index = findSymbolIndex(symbol);
  symbols_[index]->parentQueryLocation = location;

  int highestIndex = -1;
  for (const auto& sym : symbols_) {
    if (sym->parentQueryLocation == location) {
      if (sym->parentQueryIndex > highestIndex) {
        highestIndex = sym->parentQueryIndex;
      }
    }
  }
  symbols_[index]->parentQueryIndex = highestIndex + 1;
}

void SymbolTable::addAlias(const std::string& alias, const SymbolInfo* symbol) {
  auto index = findSymbolIndex(*symbol);
  symbols_[index]->alias = alias;
  symbolsByName_.insert(std::make_pair(alias, index));
}

const SymbolInfo* SymbolTable::lookupSymbolByName(
    const std::string& name) const {
  auto itr = symbolsByName_.find(name);
  if (itr == symbolsByName_.end()) {
    return nullptr;
  }
  return symbols_[itr->second].get();
}

std::vector<const SymbolInfo*> SymbolTable::lookupSymbolsByLocation(
    const Location& location) const {
  std::vector<const SymbolInfo*> symbols;
  auto [begin, end] = symbolsByLocation_.equal_range(location);
  for (auto itr = begin; itr != end; ++itr) {
    symbols.push_back(symbols_[itr->second].get());
  }
  return symbols;
}

const SymbolInfo* SymbolTable::lookupSymbolByLocationAndType(
    const Location& location,
    SymbolType type) const {
  return lookupSymbolByLocationAndTypes(location, {type});
}

const SymbolInfo* SymbolTable::lookupSymbolByLocationAndTypes(
    const Location& location,
    std::unordered_set<SymbolType> types) const {
  auto [begin, end] = symbolsByLocation_.equal_range(location);
  for (auto itr = begin; itr != end; ++itr) {
    auto symbol = symbols_[itr->second].get();
    if (types.find(symbol->type) != types.end()) {
      return symbol;
    }
  }
  return nullptr;
}

const SymbolInfo* SymbolTable::lookupSymbolByParentQueryAndType(
    const Location& location,
    int index,
    SymbolType type) const {
  for (const auto& symbol : symbols_) {
    if (symbol->parentQueryLocation == location &&
        symbol->parentQueryIndex == index && symbol->type == type) {
      return symbol.get();
    }
  }
  return nullptr;
}

const SymbolInfo& SymbolTable::nthSymbolByType(uint32_t n, SymbolType type)
    const {
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
  return {this, symbols_.size()};
}

std::string SymbolTable::toDebugString() const {
  std::stringstream result;
  bool textAlreadyWritten = false;
  int32_t relationCount = 0;
  for (const auto& symbol : symbols_) {
    if (symbol->type != SymbolType::kRelation) {
      continue;
    }
    auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbol->blob);
    result << std::left << std::setw(4) << relationCount++;
    result << std::left << std::setw(20) << symbol->name;
    if (!relationData->subQueryPipelines.empty()) {
      result << " SQC=" << relationData->subQueryPipelines.size();
    }
    result << std::endl;

    int32_t fieldNum = 0;
    for (const auto& field : relationData->fieldReferences) {
      result << "    " << std::setw(4) << fieldNum++ << "  ";
      if (field->schema != nullptr) {
        result << field->schema->name << ".";
      }
      result << field->name;
      if (!field->alias.empty()) {
        result << " " << field->alias;
      }
      result << std::endl;
    }

    for (const auto& field : relationData->generatedFieldReferences) {
      result << "   g" << std::setw(4) << fieldNum++ << "  ";
      if (field->schema != nullptr) {
        result << field->schema->name << ".";
      }
      result << field->name;
      if (relationData->generatedFieldReferenceAlternativeExpression.find(
              fieldNum) !=
          relationData->generatedFieldReferenceAlternativeExpression.end()) {
        result << " "
               << relationData
                      ->generatedFieldReferenceAlternativeExpression[fieldNum];
      } else if (!field->alias.empty()) {
        result << " " << field->alias;
      }
      result << std::endl;
    }

    int32_t outputFieldNum = 0;
    for (const auto& field : relationData->outputFieldReferences) {
      result << "   o" << std::setw(4) << outputFieldNum++ << "  ";
      if (field->schema != nullptr) {
        result << field->schema->name << ".";
      }
      result << field->name;
      if (!field->alias.empty()) {
        result << " " << field->alias;
      }
      result << std::endl;
    }
    textAlreadyWritten = true;
  }
  if (textAlreadyWritten) {
    result << std::endl;
  }
  return result.str();
}
