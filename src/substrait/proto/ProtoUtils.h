/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>

#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/plan.pb.h"

namespace substrait::proto {

std::string PlanRelTypeCaseName(::substrait::proto::PlanRel::RelTypeCase num);

std::string RelTypeCaseName(::substrait::proto::Rel::RelTypeCase num);

} // namespace substrait::proto