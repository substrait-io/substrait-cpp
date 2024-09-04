/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/SaveBinary.h"

#include <absl/strings/str_join.h>
#include <fmt/format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>

#ifdef _WIN32
#include <io.h>
#else
#include <sys/fcntl.h>
#include <sys/stat.h>
#endif

#include <fstream>

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/StringManipulation.h"
#include "substrait/textplan/SymbolTablePrinter.h"
#include "substrait/textplan/converter/ParseBinary.h"

namespace io::substrait::textplan {

absl::Status savePlanToBinary(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename) {
  int outputFileDescriptor =
      creat(std::string{output_filename}.c_str(), S_IREAD | S_IWRITE);
  if (outputFileDescriptor == -1) {
    return absl::ErrnoToStatus(
        errno,
        fmt::format("Failed to open file {} for writing", output_filename));
  }
  auto stream =
      new google::protobuf::io::FileOutputStream(outputFileDescriptor);

  if (!plan.SerializeToZeroCopyStream(stream)) {
    return ::absl::UnknownError("Failed to write plan to stream.");
  }

  if (!stream->Close()) {
    return absl::AbortedError("Failed to close file descriptor.");
  }
  delete stream;
  return absl::OkStatus();
}

absl::Status savePlanToJson(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename) {
  std::ofstream stream(std::string{output_filename});
  if ((stream.fail())) {
    return absl::UnavailableError(
        fmt::format("Failed to open file {} for writing", output_filename));
  }

  std::string output;
  auto status = ::google::protobuf::util::MessageToJsonString(plan, &output);
  if (!status.ok()) {
    return absl::UnknownError("Failed to save plan as a JSON protobuf.");
  }
  stream << output;
  stream.close();
  if (stream.fail()) {
    return absl::UnknownError("Failed to write the plan as a JSON protobuf.");
  }
  return absl::OkStatus();
}

absl::Status savePlanToText(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename) {
  std::ofstream stream(std::string{output_filename});
  if ((stream.fail())) {
    return absl::UnavailableError(
        fmt::format("Failed to open file {} for writing", output_filename));
  }

  auto result = parseBinaryPlan(plan);
  auto errors = result.getAllErrors();
  if (!errors.empty()) {
    return absl::UnknownError(absl::StrJoin(errors, ""));
  }
  SubstraitErrorListener errorListener;
  stream << SymbolTablePrinter::outputToText(
      result.getSymbolTable(), &errorListener);
  stream.close();
  if (stream.fail()) {
    return absl::UnknownError("Failed to write the plan as text.");
  }
  if (!errorListener.getErrorMessages().empty()) {
    return absl::UnknownError(fmt::format(
        "Errors while writing to text: {}",
        absl::StrJoin(errorListener.getErrorMessages(), "\n")));
  }
  return absl::OkStatus();
}

absl::Status savePlanToProtoText(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename) {
  int outputFileDescriptor =
      creat(std::string{output_filename}.c_str(), S_IREAD | S_IWRITE);
  if (outputFileDescriptor == -1) {
    return absl::ErrnoToStatus(
        errno,
        fmt::format("Failed to open file {} for writing", output_filename));
  }
  auto stream =
      new google::protobuf::io::FileOutputStream(outputFileDescriptor);

  if (!::google::protobuf::TextFormat::Print(plan, stream)) {
    return absl::UnknownError("Failed to save plan as a text protobuf.");
  }

  if (!stream->Close()) {
    return absl::AbortedError("Failed to close file descriptor.");
  }
  delete stream;
  return absl::OkStatus();
}

} // namespace io::substrait::textplan
