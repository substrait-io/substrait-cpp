/* SPDX-License-Identifier: Apache-2.0 */

#include <substrait/common/Io.h>

extern "C" {
	// Load a Substrait plan (in any format) from disk.
	// Returns error message on failure, otherwise returns nullptr and
	// updates the provided buffer.
	const char* load_substrait_plan(char* planBuffer, int32_t planBufferLen,
	                                const char* filename) {
		auto planOrError = io::substrait::loadPlan(filename);
		if (!planOrError.ok()) {
			return planOrError.status().message().data();
		}
		auto plan = *planOrError;
		// MEGAHACK -- If space will fit update the planBuffer.
		return nullptr;
	}

	// Write a Substrait plan to disk in the specified format.
	const char* save_substrait_plan(const char* planData, int32_t planDataLen,
	                                const char* filename,
	                                io::substrait::PlanFileFormat format) {
		// MEGAHACK -- Convert the planData into a Plan proto.
		::substrait::proto::Plan plan;
		auto result = io::substrait::savePlan(plan, filename, format);
		if (!result.ok()) {
			return result.message().data();
		}
		return nullptr;
	}
}
