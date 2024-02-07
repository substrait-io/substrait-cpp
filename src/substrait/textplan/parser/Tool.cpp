/* SPDX-License-Identifier: Apache-2.0 */

#include <getopt.h>
#include <sstream>

#include "substrait/textplan/SymbolTablePrinter.h"
#include "substrait/textplan/parser/ParseText.h"

namespace io::substrait::textplan {
namespace {

void readText(const char* filename) {
  auto stream = io::substrait::textplan::loadTextFile(filename);
  if (!stream.has_value()) {
    std::cerr << "An error occurred while reading: " << filename << std::endl;
    return;
  }
  auto parseResult = io::substrait::textplan::parseStream(&*stream);
  if (!parseResult.successful()) {
    for (const std::string& msg : parseResult.getAllErrors()) {
      std::cout << msg << std::endl;
    }
    return;
  }

  SubstraitErrorListener errorListener;
  auto text = SymbolTablePrinter::outputToText(
      parseResult.getSymbolTable(), &errorListener);
  if (errorListener.hasErrors()) {
    for (const std::string& msg : errorListener.getErrorMessages()) {
      std::cout << msg << std::endl;
    }
    return;
  }

  std::cout << text;
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
    printf("Usage:  planparser <file1> <file2> ...\n");
    return EXIT_FAILURE;
  }

  int currArg = optind;
  for (; currArg < argc; currArg++) {
    printf("===== %s =====\n", argv[currArg]);
    io::substrait::textplan::readText(argv[currArg]);
  }

  return EXIT_SUCCESS;
}
