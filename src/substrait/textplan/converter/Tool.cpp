/* SPDX-License-Identifier: Apache-2.0 */

#include <getopt.h>

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
    printf("===== %s =====\n", argv[currArg]);
    io::substrait::textplan::convertJsonToText(argv[currArg]);
  }

  return EXIT_SUCCESS;
}
