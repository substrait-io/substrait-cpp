/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "substrait/textplan/ParseResult.h"

namespace substrait::proto {
class Plan;
}

namespace io::substrait::textplan {

ParseResult ParseBinaryPlan(const ::substrait::proto::Plan& plan);

} // namespace io::substrait::textplan
