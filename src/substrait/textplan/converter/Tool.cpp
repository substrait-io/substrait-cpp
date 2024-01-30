/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _WIN32
#include <glob.h>
#endif

#include <sstream>

#include "substrait/common/Io.h"
#include "substrait/textplan/SymbolTablePrinter.h"
#include "substrait/textplan/converter/LoadBinary.h"
#include "substrait/textplan/converter/ParseBinary.h"

namespace io::substrait::textplan {
namespace {

void convertPlanToText(const char* filename) {
  auto planOrError = loadPlan(filename);
  if (!planOrError.ok()) {
    std::cerr << planOrError.status() << std::endl;
    return;
  }

  auto result = parseBinaryPlan(*planOrError);
  SubstraitErrorListener errorListener;
  auto textResult =
      SymbolTablePrinter::outputToText(result.getSymbolTable(), &errorListener);
  result.addErrors(errorListener.getErrorMessages());
  auto errors = result.getAllErrors();
  if (!errors.empty()) {
    for (const auto& err : errors) {
      std::cerr << err << std::endl;
    }
  }
  std::cout << textResult;
}

} // namespace
} // namespace io::substrait::textplan

int main(int argc, char* argv[]) {
  if (argc <= 1) {
    printf("Usage:  planconverter <file1> <file2> ...\n");
    return EXIT_FAILURE;
  }

#ifdef _WIN32
  for (int currArg = 1; currArg < argc; currArg++) {
    printf("===== %s =====\n", argv[currArg]);
    io::substrait::textplan::convertPlanToText(argv[currArg]);
  }
#else
  for (int currArg = 1; currArg < argc; currArg++) {
    glob_t globResult;
    glob(argv[currArg], GLOB_TILDE, nullptr, &globResult);
    for (size_t i = 0; i < globResult.gl_pathc; i++) {
      printf("===== %s =====\n", globResult.gl_pathv[i]);
      io::substrait::textplan::convertPlanToText(globResult.gl_pathv[i]);
    }
  }
#endif

  return EXIT_SUCCESS;
}
