/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <substrait/proto/algebra.pb.h>
#include <substrait/proto/plan.pb.h>

#include <string>

namespace substrait::proto {

std::string planRelTypeCaseName(::substrait::proto::PlanRel::RelTypeCase num);

std::string relTypeCaseName(::substrait::proto::Rel::RelTypeCase num);

} // namespace substrait::proto
