/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <google/protobuf/util/json_util.h>

#include "PlanConverter.h"
#include "substrait/algebra.pb.h"
#include "substrait/common/Exceptions.h"
#include "substrait/plan.pb.h"

namespace {

std::string shortName(std::string str) {
  auto loc = str.find(':');
  if (loc != std::string::npos) {
    return str.substr(0, loc);
  }
  return str;
}

} // namespace

std::string readFromFile(const std::string& msgPath) {
  // Read json file and resume the Substrait plan.
  std::ifstream msgJson(msgPath);
  if (msgJson.fail()) {
    auto currdir = std::filesystem::current_path().string();
    SUBSTRAIT_FAIL(
        "Failed to open file {} when running in {}: {}",
        msgPath,
        currdir,
        strerror(errno));
  }
  std::stringstream buffer;
  buffer << msgJson.rdbuf();
  return buffer.str();
}

void PlanConverter::loadFromJSON(const std::string& json) {
  auto status = google::protobuf::util::JsonStringToMessage(json, &plan_);
  if (!status.ok()) {
    SUBSTRAIT_FAIL(
        "Failed to parse Substrait JSON: {}", status.message().ToString());
  }
}

void PlanConverter::loadFromText(const std::string& text) {
  SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
}

void PlanConverter::loadFromBinary(const std::string& blob) {
  SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
}

std::string PlanConverter::functionsToText(const substrait::Plan& plan) {
  std::string text;
  // MEGAHACK -- Deal with grouping extensions by extension space
  // (extension_uri_reference). MEGAHACK -- Also deal with the fact that there
  // may be no extension space provided.
  if (plan.extensions_size() == 0) {
    return text;
  }
  text += "extension_space {\n";
  auto extensions = plan.extensions();
  std::sort(
      extensions.begin(),
      extensions.end(),
      [](const substrait::extensions::SimpleExtensionDeclaration& a,
         const substrait::extensions::SimpleExtensionDeclaration& b) {
        return a.extension_function().function_anchor() <
            b.extension_function().function_anchor();
      });
  for (const auto& ext : plan.extensions()) {
    text += "  function " + ext.extension_function().name() + " as " +
        shortName(ext.extension_function().name()) + ";\n";
  }
  text + " }\n";
  return text;
}

namespace {
std::string extractLocalFile(
    const substrait::ReadRel::LocalFiles::FileOrFiles& item) {
  std::string text;
  switch (item.path_type_case()) {
    case substrait::ReadRel::LocalFiles::FileOrFiles::kUriPath:
      text += "uri_path: \"" + item.uri_path() + "\"";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kUriPathGlob:
      text += "uri_path_glob: \"" + item.uri_path_glob() + "\"";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kUriFile:
      text += "uri_file: \"" + item.uri_file() + "\"";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kUriFolder:
      text += "uri_folder: \"" + item.uri_folder() + "\"";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::PATH_TYPE_NOT_SET:
    default:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
      return "";
  }
  if (item.partition_index() != 0) {
    text += " partition_index: " + std::to_string(item.partition_index());
  }
  if (item.start() != 0) {
    text += " start: " + std::to_string(item.start());
  }
  if (item.length() != 0) {
    text += " length: " + std::to_string(item.length());
  }
  switch (item.file_format_case()) {
    case substrait::ReadRel::LocalFiles::FileOrFiles::kParquet:
      text += " parquest: {}";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kArrow:
      text += " arrow: {}";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kOrc:
      text += " orc: {}";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kExtension:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
      return "";
    case substrait::ReadRel::LocalFiles::FileOrFiles::kDwrf:
      text += " dwrf: {}";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::FILE_FORMAT_NOT_SET:
    default:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
      return "";
  }
  return text;
}

std::string extractReadType(const substrait::ReadRel& relation) {
  switch (relation.read_type_case()) {
    case substrait::ReadRel::kLocalFiles: {
      std::string text = "source local_files {\n";
      text += "  items = [\n";
      for (const auto& item : relation.local_files().items()) {
        text += "    {" + extractLocalFile(item) + "}\n";
      }
      text += "  ]\n";
      text += "}\n";
      // MEGAHACK -- Add support for advanced extensions.
      return text;
    }
    case substrait::ReadRel::kVirtualTable:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
    case substrait::ReadRel::kNamedTable:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
    case substrait::ReadRel::kExtensionTable:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
    case substrait::ReadRel::READ_TYPE_NOT_SET:
    default:
      return "";
  }
}

std::string extractSources(const substrait::Rel& relation) {
  switch (relation.rel_type_case()) {
    case substrait::Rel::RelTypeCase::kRead:
      return extractReadType(relation.read());
    case substrait::Rel::RelTypeCase::kFilter:
      return extractSources(relation.filter().input());
    case substrait::Rel::RelTypeCase::kFetch:
      return extractSources(relation.fetch().input());
    case substrait::Rel::RelTypeCase::kAggregate:
      return extractSources(relation.aggregate().input());
    case substrait::Rel::RelTypeCase::kSort:
      return extractSources(relation.sort().input());
    case substrait::Rel::RelTypeCase::kJoin:
      return extractSources(relation.join().left()) +
          extractSources(relation.join().right());
    case substrait::Rel::RelTypeCase::kProject:
      return extractSources(relation.project().input());
    case substrait::Rel::RelTypeCase::kSet: {
      std::string text;
      for (const auto& rel : relation.set().inputs()) {
        text += extractSources(rel);
      }
      return text;
    }
    case substrait::Rel::RelTypeCase::kExtensionSingle:
      return extractSources(relation.extension_single().input());
    case substrait::Rel::RelTypeCase::kExtensionMulti: {
      std::string text;
      for (const auto& rel : relation.extension_multi().inputs()) {
        text += extractSources(rel);
      }
      return text;
    }
    case substrait::Rel::RelTypeCase::kExtensionLeaf:
      return "";
    case substrait::Rel::RelTypeCase::kCross:
      return extractSources(relation.cross().left()) +
          extractSources(relation.cross().right());
#if 0
    case substrait::Rel::RelTypeCase::kHashJoin:
      return extractSources(relation.hash_join().left()) + extractSources(relation.hash_join().right());
    case substrait::Rel::RelTypeCase::kMergeJoin:
      return extractSources(relation.merge_join().left()) + extractSources(relation.merge_join().right());
#endif
    case substrait::Rel::REL_TYPE_NOT_SET:
    default:
      return "";
  }
}

std::string extractSources(const substrait::PlanRel& relation) {
  // MEGAHACK -- Use rel_type_case here.
  if (relation.has_root()) {
    return extractSources(relation.root().input());
  } else if (relation.has_rel()) {
    return extractSources(relation.rel());
  }
  return "";
}

class Pipeline {
 public:
  Pipeline() = default;

  void add(const std::string& node) {
    nodes_.push_back(node);
  }

  std::string toString() {
    std::string text;
    for (auto it = nodes_.rbegin(); it != nodes_.rend(); ++it) {
      if (!text.empty()) {
        text += " -> ";
      }
      text += *it;
    }
    text += ";\n";
    return text;
  }

 private:
  std::vector<std::string> nodes_;
};

class PipelineCollector {
 public:
  PipelineCollector() = default;

  Pipeline* add(const std::string& name) {
    auto pipeline = std::make_shared<Pipeline>();
    pipelines_.emplace_back(pipeline);
    pipeline->add(name);
    return pipeline.get();
  }

  std::string getUniqueName(const std::string& base_name) {
    if (names_.find(base_name) == names_.end()) {
      names_.insert(std::make_pair(base_name, 1));
      return base_name;
    }
    names_[base_name]++;
    return base_name + std::to_string(names_[base_name]);
  }

  std::string toString() {
    std::string text = "pipelines {\n";
    for (auto it = pipelines_.rbegin(); it != pipelines_.rend(); ++it) {
      const auto& pipeline = *it;
      text += "  " + pipeline->toString();
    }
    text += "}\n";
    return text;
  }

 private:
  std::vector<std::shared_ptr<Pipeline>> pipelines_;
  std::map<std::string, int32_t > names_;
};

} // namespace

std::string PlanConverter::sourcesToText(const substrait::Plan& plan) {
  std::string text;
  for (const auto& relation : plan.relations()) {
    text += extractSources(relation);
  }
  return text;
}

std::string PlanConverter::schemasToText(const substrait::Plan& plan) {
  std::string text;
  return text;
}

std::string PlanConverter::relationsToText(const substrait::Plan& plan) {
  std::string text;
  return text;
}

void visitPipelines(
    const substrait::Rel& relation,
    PipelineCollector* collector,
    Pipeline* pipeline) {
  switch (relation.rel_type_case()) {
    case substrait::Rel::RelTypeCase::kRead:
      pipeline->add(collector->getUniqueName("read"));
      break;
    case substrait::Rel::RelTypeCase::kFilter: {
      pipeline->add(collector->getUniqueName("filter"));
      visitPipelines(relation.filter().input(), collector, pipeline);
      break;
    }
    case substrait::Rel::RelTypeCase::kFetch: {
      pipeline->add(collector->getUniqueName("fetch"));
      visitPipelines(relation.fetch().input(), collector, pipeline);
      break;
    }
    case substrait::Rel::RelTypeCase::kAggregate: {
      pipeline->add(collector->getUniqueName("aggregate"));
      visitPipelines(relation.aggregate().input(), collector, pipeline);
      break;
    }
    case substrait::Rel::RelTypeCase::kSort: {
      pipeline->add(collector->getUniqueName("sort"));
      visitPipelines(relation.sort().input(), collector, pipeline);
      break;
    }
    case substrait::Rel::RelTypeCase::kJoin: {
      auto name = collector->getUniqueName("join");
      pipeline->add(name);
      visitPipelines(relation.join().left(), collector, collector->add(name));
      visitPipelines(relation.join().right(), collector, collector->add(name));
      break;
    }
    case substrait::Rel::RelTypeCase::kProject: {
      pipeline->add(collector->getUniqueName("project"));
      visitPipelines(relation.project().input(), collector, pipeline);
      break;
    }
    case substrait::Rel::RelTypeCase::kSet: {
      auto name = collector->getUniqueName("set");
      pipeline->add(name);
      for (const auto& rel : relation.set().inputs()) {
        visitPipelines(rel, collector, collector->add(name));
      }
      break;
    }
    case substrait::Rel::RelTypeCase::kExtensionSingle: {
      pipeline->add(collector->getUniqueName("extension_single"));
      visitPipelines(relation.extension_single().input(), collector, pipeline);
      break;
    }
    case substrait::Rel::RelTypeCase::kExtensionMulti: {
      auto name = collector->getUniqueName("extension_multi");
      pipeline->add(name);
      for (const auto& rel : relation.extension_multi().inputs()) {
        visitPipelines(rel, collector, collector->add(name));
      }
      break;
    }
    case substrait::Rel::RelTypeCase::kExtensionLeaf:
      pipeline->add(collector->getUniqueName("extension_leaf"));
      break;
    case substrait::Rel::RelTypeCase::kCross: {
      auto name = collector->getUniqueName("cross");
      pipeline->add(name);
      visitPipelines(relation.cross().left(), collector, collector->add(name));
      visitPipelines(relation.cross().right(), collector, collector->add(name));
      break;
    }
#if 0
    case substrait::Rel::RelTypeCase::kHashJoin: {
      auto name = collector->getUniqueName("hash_join");
      pipeline->add(name);
      visitPipelines(
          relation.hash_join().left(), collector, collector->add(name));
      visitPipelines(
          relation.hash_join().right(), collector, collector->add(name));
      break;
    }
    case substrait::Rel::RelTypeCase::kMergeJoin: {
      auto name = collector->getUniqueName("merge_join");
      pipeline->add(name);
      visitPipelines(
          relation.merge_join().left(), collector, collector->add(name));
      visitPipelines(
          relation.merge_join().right(), collector, collector->add(name));
      break;
    }
#endif
    case substrait::Rel::REL_TYPE_NOT_SET:
    default:
      break;
  }
}

void visitPipelines(
    const substrait::PlanRel& relation,
    PipelineCollector* collector) {
  switch (relation.rel_type_case()) {
    case substrait::PlanRel::kRoot:
      // MEGAHACK -- Figure out whether this "node" should be listed as being part of the pipeline.
      visitPipelines(
          relation.root().input(),
          collector,
          collector->add(collector->getUniqueName("root")));
      break;
    case substrait::PlanRel::kRel:
      // MEGAHACK -- Figure out whether this "node" should be listed as being part of the pipeline.
      visitPipelines(
          relation.rel(),
          collector,
          collector->add(collector->getUniqueName("rel")));
      break;
    case substrait::PlanRel::REL_TYPE_NOT_SET:
    default:
      return;
  }
}

std::string PlanConverter::pipelinesToText(const substrait::Plan& plan) {
  PipelineCollector collector;
  for (const auto& relation : plan.relations()) {
    visitPipelines(relation, &collector);
  }
  return collector.toString();
}

std::string PlanConverter::toString() {
  std::string text;
  text += pipelinesToText(plan_);
  if (!text.empty()) {
    text += "\n";
  }
  text += relationsToText(plan_);
  if (!text.empty()) {
    text += "\n";
  }
  text += schemasToText(plan_);
  if (!text.empty()) {
    text += "\n";
  }
  text += sourcesToText(plan_);
  if (!text.empty()) {
    text += "\n";
  }
  text += functionsToText(plan_);
  return text;
}
