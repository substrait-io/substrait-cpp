/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>

#include "substrait/proto/type.pb.h"

namespace io::substrait::textplan {

std::string typeToText(const ::substrait::proto::Type& type);

} // namespace io::substrait::textplan
