
// SPDX-License-Identifier: Apache-2.0


// Generated from SubstraitPlanLexer.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"


#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


namespace io::substrait::textplan {


class  SubstraitPlanLexer : public antlr4::Lexer {
public:
  enum {
    SPACES = 1, EXTENSION_SPACE = 2, FUNCTION = 3, AS = 4, NAMED = 5, SCHEMA = 6, 
    RELATION = 7, PIPELINES = 8, COMMON = 9, BASE_SCHEMA = 10, FILTER = 11, 
    PROJECTION = 12, EXPRESSION = 13, ADVANCED_EXTENSION = 14, GROUPING = 15, 
    MEASURE = 16, INVOCATION = 17, SORT = 18, BY = 19, COUNT = 20, TYPE = 21, 
    EMIT = 22, SUBQUERY = 23, EXISTS = 24, UNIQUE = 25, IN = 26, ALL = 27, 
    ANY = 28, COMPARISON = 29, VIRTUAL_TABLE = 30, LOCAL_FILES = 31, NAMED_TABLE = 32, 
    EXTENSION_TABLE = 33, SOURCE = 34, ROOT = 35, ITEMS = 36, NAMES = 37, 
    URI_FILE = 38, URI_PATH = 39, URI_PATH_GLOB = 40, URI_FOLDER = 41, PARTITION_INDEX = 42, 
    START = 43, LENGTH = 44, ORC = 45, PARQUET = 46, NULLVAL = 47, TRUEVAL = 48, 
    FALSEVAL = 49, LIST = 50, MAP = 51, STRUCT = 52, ARROW = 53, COLON = 54, 
    SEMICOLON = 55, LEFTBRACE = 56, RIGHTBRACE = 57, LEFTPAREN = 58, RIGHTPAREN = 59, 
    COMMA = 60, PERIOD = 61, EQUAL = 62, LEFTBRACKET = 63, RIGHTBRACKET = 64, 
    UNDERSCORE = 65, MINUS = 66, LEFTANGLEBRACKET = 67, RIGHTANGLEBRACKET = 68, 
    QUESTIONMARK = 69, ATSIGN = 70, IDENTIFIER = 71, NUMBER = 72, STRING = 73, 
    SINGLE_LINE_COMMENT = 74, URI = 75
  };

  enum {
    CommentsChannel = 2, DirectiveChannel = 3
  };

  enum {
    EXTENSIONS = 1
  };

  explicit SubstraitPlanLexer(antlr4::CharStream *input);

  ~SubstraitPlanLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

}  // namespace io::substrait::textplan
