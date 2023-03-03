/* SPDX-License-Identifier: Apache-2.0 */

#include <getopt.h>

#include <iostream>

#include "substrait/textplan/SymbolTablePrinter.h"
#include "substrait/textplan/converter/LoadBinary.h"
#include "substrait/textplan/converter/ParseBinary.h"

namespace io::substrait::textplan {
namespace {

void ConvertJsonToText(const char* filename) {
  std::string json = ReadFromFile(filename);
  auto planOrError = LoadFromJson(json);
  if (!planOrError.ok()) {
    std::cerr << "An error occurred while reading: " << filename << std::endl;
    for (const auto& err : planOrError.errors()) {
      std::cerr << err << std::endl;
    }
    return;
  }

  auto result = ParseBinaryPlan(*planOrError);
  std::cout << SymbolTablePrinter::outputToText(result.getSymbolTable());
}

} // namespace
} // namespace io::substrait::textplan

int main(int argc, char* argv[]) {
  while (true) {
    int option_index = 0;
    static struct option long_options[] = {{nullptr, 0, nullptr, 0}};

    int c = getopt_long(argc, argv, "", long_options, &option_index);
    if (c == -1) {
      break;
    }
  }

  if (optind >= argc) {
    printf("Usage:  planconverter <file1> <file2> ...\n");
    return EXIT_FAILURE;
  }

  int curr_arg = optind;
  for (; curr_arg < argc; curr_arg++) {
    printf("===== %s =====\n", argv[curr_arg]);
    io::substrait::textplan::ConvertJsonToText(argv[curr_arg]);
  }

  return EXIT_SUCCESS;
}
