/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string_view>

namespace io::substrait::textplan {

// Yields true if the string 'haystack' starts with the string 'needle'.
bool startsWith(std::string_view haystack, std::string_view needle);

// Returns true if the string 'haystack' ends with the string 'needle'.
bool endsWith(std::string_view haystack, std::string_view needle);

}  // namespace io::substrait::textplan
