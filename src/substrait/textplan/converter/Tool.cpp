/* SPDX-License-Identifier: Apache-2.0 */

#include <getopt.h>
#include <glob.h>

#include <iostream>

#include "substrait/textplan/SymbolTablePrinter.h"
#include "substrait/textplan/converter/LoadBinary.h"
#include "substrait/textplan/converter/ParseBinary.h"

namespace io::substrait::textplan {
namespace {

void convertJsonToText(const char* filename) {
  std::string json = readFromFile(filename);
  auto planOrError = loadFromJson(json);
  if (!planOrError.ok()) {
    std::cerr << "An error occurred while reading: " << filename << std::endl;
    for (const auto& err : planOrError.errors()) {
      std::cerr << err << std::endl;
    }
    return;
  }

  auto result = parseBinaryPlan(*planOrError);
  auto errors = result.getAllErrors();
  if (!errors.empty()) {
    for (const auto& err : errors) {
      std::cerr << err << std::endl;
    }
  }
  std::cout << SymbolTablePrinter::outputToText(result.getSymbolTable());
}

} // namespace
} // namespace io::substrait::textplan

int main(int argc, char* argv[]) {
  while (true) {
    int optionIndex = 0;
    static struct option longOptions[] = {{nullptr, 0, nullptr, 0}};

    int c = getopt_long(argc, argv, "", longOptions, &optionIndex);
    if (c == -1) {
      break;
    }
  }

  if (optind >= argc) {
    printf("Usage:  planconverter <file1> <file2> ...\n");
    return EXIT_FAILURE;
  }

  int currArg = optind;
  for (; currArg < argc; currArg++) {
    glob_t globResult;
    glob(argv[currArg], GLOB_TILDE, nullptr, &globResult);
    for (size_t i = 0; i < globResult.gl_pathc; i++) {
      printf("===== %s =====\n", globResult.gl_pathv[i]);
      io::substrait::textplan::convertJsonToText(globResult.gl_pathv[i]);
    }
  }

  return EXIT_SUCCESS;
}
