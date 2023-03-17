/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <vector>

#include <google/protobuf/message.h>

namespace io::substrait::textplan {

class HierarchyStack {
 public:
  void push(const ::google::protobuf::Message* protoAddr) {
    stack_.push_back(protoAddr);
  }

  void pop() {
    stack_.pop_back();
  }

  const ::google::protobuf::Message* getEnclosingScope() {
    if (stack_.empty())
      return nullptr;
    return stack_[stack_.size()-1];
  }

 private:
  std::vector<const ::google::protobuf::Message*> stack_;
};

class HierarchyScope {
 public:
  HierarchyScope(
      HierarchyStack* stack,
      const ::google::protobuf::Message* protoAddr)
      : stack_(stack) {
    stack_->push(protoAddr);
  };
  ~HierarchyScope() {
    stack_->pop();
  };

 private:
  HierarchyStack* stack_;
};

} // namespace io::substrait::textplan