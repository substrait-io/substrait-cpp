
// Generated from SubstraitPlanParser.g4 by ANTLR 4.13.2


#include "SubstraitPlanParserVisitor.h"

#include "SubstraitPlanParser.h"


#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


using namespace antlrcpp;
using namespace io::substrait::textplan;

using namespace antlr4;

namespace {

struct SubstraitPlanParserStaticData final {
  SubstraitPlanParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  SubstraitPlanParserStaticData(const SubstraitPlanParserStaticData&) = delete;
  SubstraitPlanParserStaticData(SubstraitPlanParserStaticData&&) = delete;
  SubstraitPlanParserStaticData& operator=(const SubstraitPlanParserStaticData&) = delete;
  SubstraitPlanParserStaticData& operator=(SubstraitPlanParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag substraitplanparserParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<SubstraitPlanParserStaticData> substraitplanparserParserStaticData = nullptr;

void substraitplanparserParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (substraitplanparserParserStaticData != nullptr) {
    return;
  }
#else
  assert(substraitplanparserParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<SubstraitPlanParserStaticData>(
    std::vector<std::string>{
      "plan", "plan_detail", "pipelines", "pipeline", "relation", "root_relation", 
      "relation_type", "relation_ref", "relation_filter_behavior", "measure_detail", 
      "relation_detail", "expression", "expression_list", "constant", "literal_basic_type", 
      "literal_complex_type", "literal_specifier", "map_literal", "map_literal_value", 
      "struct_literal", "column_name", "source_reference", "file_location", 
      "file_detail", "file", "local_files_detail", "named_table_detail", 
      "schema_definition", "schema_item", "source_definition", "read_type", 
      "extensionspace", "function", "sort_field", "name", "signature", "id", 
      "simple_id"
    },
    std::vector<std::string>{
      "", "", "'EXTENSION_SPACE'", "'FUNCTION'", "'AS'", "'NAMED'", "'SCHEMA'", 
      "'RELATION'", "'PIPELINES'", "'COMMON'", "'BASE_SCHEMA'", "'FILTER'", 
      "'PROJECTION'", "'EXPRESSION'", "'ADVANCED_EXTENSION'", "'GROUPING'", 
      "'MEASURE'", "'INVOCATION'", "'SORT'", "'BY'", "'COUNT'", "'TYPE'", 
      "'EMIT'", "'SUBQUERY'", "'EXISTS'", "'UNIQUE'", "'IN'", "'ALL'", "'ANY'", 
      "", "'VIRTUAL_TABLE'", "'LOCAL_FILES'", "'NAMED_TABLE'", "'EXTENSION_TABLE'", 
      "'SOURCE'", "'ROOT'", "'ITEMS'", "'NAMES'", "'URI_FILE'", "'URI_PATH'", 
      "'URI_PATH_GLOB'", "'URI_FOLDER'", "'PARTITION_INDEX'", "'START'", 
      "'LENGTH'", "'ORC'", "'PARQUET'", "'NULL'", "'TRUE'", "'FALSE'", "'LIST'", 
      "'MAP'", "'STRUCT'", "'->'", "':'", "';'", "'{'", "'}'", "'('", "')'", 
      "','", "'.'", "'='", "'['", "']'", "'_'", "'-'", "'<'", "'>'", "'\\u003F'", 
      "'@'"
    },
    std::vector<std::string>{
      "", "SPACES", "EXTENSION_SPACE", "FUNCTION", "AS", "NAMED", "SCHEMA", 
      "RELATION", "PIPELINES", "COMMON", "BASE_SCHEMA", "FILTER", "PROJECTION", 
      "EXPRESSION", "ADVANCED_EXTENSION", "GROUPING", "MEASURE", "INVOCATION", 
      "SORT", "BY", "COUNT", "TYPE", "EMIT", "SUBQUERY", "EXISTS", "UNIQUE", 
      "IN", "ALL", "ANY", "COMPARISON", "VIRTUAL_TABLE", "LOCAL_FILES", 
      "NAMED_TABLE", "EXTENSION_TABLE", "SOURCE", "ROOT", "ITEMS", "NAMES", 
      "URI_FILE", "URI_PATH", "URI_PATH_GLOB", "URI_FOLDER", "PARTITION_INDEX", 
      "START", "LENGTH", "ORC", "PARQUET", "NULLVAL", "TRUEVAL", "FALSEVAL", 
      "LIST", "MAP", "STRUCT", "ARROW", "COLON", "SEMICOLON", "LEFTBRACE", 
      "RIGHTBRACE", "LEFTPAREN", "RIGHTPAREN", "COMMA", "PERIOD", "EQUAL", 
      "LEFTBRACKET", "RIGHTBRACKET", "UNDERSCORE", "MINUS", "LEFTANGLEBRACKET", 
      "RIGHTANGLEBRACKET", "QUESTIONMARK", "ATSIGN", "IDENTIFIER", "NUMBER", 
      "STRING", "SINGLE_LINE_COMMENT", "URI"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,75,633,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,1,0,5,0,78,8,0,10,0,12,0,81,9,0,1,0,1,0,1,1,1,
  	1,1,1,1,1,1,1,1,1,3,1,91,8,1,1,2,1,2,1,2,1,2,1,2,5,2,98,8,2,10,2,12,2,
  	101,9,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,5,3,111,8,3,10,3,12,3,114,9,3,
  	1,4,1,4,1,4,1,4,1,4,5,4,121,8,4,10,4,12,4,124,9,4,1,4,1,4,1,5,1,5,1,5,
  	1,5,1,5,1,5,1,5,1,5,5,5,136,8,5,10,5,12,5,139,9,5,1,5,3,5,142,8,5,1,5,
  	1,5,1,5,1,6,1,6,1,7,1,7,1,7,1,7,1,7,1,7,3,7,155,8,7,1,8,1,8,1,8,1,8,1,
  	8,1,8,1,8,1,8,3,8,165,8,8,1,9,1,9,1,9,1,9,3,9,171,8,9,1,9,1,9,3,9,175,
  	8,9,1,9,1,9,3,9,179,8,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,3,
  	9,192,8,9,1,10,1,10,1,10,1,10,1,10,1,10,1,10,3,10,201,8,10,1,10,1,10,
  	1,10,1,10,1,10,1,10,1,10,1,10,3,10,211,8,10,1,10,1,10,1,10,1,10,1,10,
  	1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,5,10,227,8,10,10,10,12,10,
  	230,9,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,
  	1,10,3,10,245,8,10,1,11,1,11,1,11,1,11,1,11,3,11,252,8,11,5,11,254,8,
  	11,10,11,12,11,257,9,11,1,11,1,11,1,11,3,11,262,8,11,1,11,1,11,1,11,1,
  	11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,3,11,277,8,11,1,11,1,
  	11,1,11,1,11,1,11,1,11,1,11,1,11,5,11,287,8,11,10,11,12,11,290,9,11,1,
  	12,1,12,1,12,1,12,5,12,296,8,12,10,12,12,12,299,9,12,1,12,1,12,1,13,1,
  	13,1,13,3,13,306,8,13,1,13,1,13,1,13,3,13,311,8,13,1,13,1,13,1,13,3,13,
  	316,8,13,1,13,1,13,1,13,3,13,321,8,13,1,13,1,13,1,13,3,13,326,8,13,1,
  	13,1,13,1,13,3,13,331,8,13,1,13,1,13,1,13,3,13,336,8,13,3,13,338,8,13,
  	1,14,1,14,3,14,342,8,14,1,14,3,14,345,8,14,1,15,1,15,1,15,3,15,350,8,
  	15,1,15,1,15,3,15,354,8,15,1,15,1,15,1,15,3,15,359,8,15,1,15,1,15,3,15,
  	363,8,15,1,15,3,15,366,8,15,1,15,3,15,369,8,15,1,15,1,15,1,15,3,15,374,
  	8,15,1,15,1,15,3,15,378,8,15,1,15,1,15,5,15,382,8,15,10,15,12,15,385,
  	9,15,1,15,3,15,388,8,15,1,16,1,16,1,16,1,16,5,16,394,8,16,10,16,12,16,
  	397,9,16,1,16,1,16,1,17,1,17,1,17,1,17,5,17,405,8,17,10,17,12,17,408,
  	9,17,1,17,1,17,1,17,1,17,3,17,414,8,17,1,18,1,18,1,18,1,18,1,19,1,19,
  	1,19,1,19,5,19,424,8,19,10,19,12,19,427,9,19,1,19,1,19,1,19,1,19,3,19,
  	433,8,19,1,20,1,20,1,20,3,20,438,8,20,1,20,1,20,1,21,1,21,1,21,1,22,1,
  	22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,1,22,3,22,457,8,22,1,
  	23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,
  	23,1,23,1,23,1,23,3,23,477,8,23,1,24,1,24,5,24,481,8,24,10,24,12,24,484,
  	9,24,1,24,1,24,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,5,25,496,8,25,
  	10,25,12,25,499,9,25,1,25,3,25,502,8,25,1,25,1,25,3,25,506,8,25,1,26,
  	1,26,1,26,1,26,1,26,1,26,1,26,1,26,5,26,516,8,26,10,26,12,26,519,9,26,
  	1,26,3,26,522,8,26,1,26,3,26,525,8,26,1,27,1,27,1,27,1,27,5,27,531,8,
  	27,10,27,12,27,534,9,27,1,27,1,27,1,28,1,28,1,28,1,28,3,28,542,8,28,1,
  	28,1,28,1,29,1,29,1,29,1,30,1,30,1,30,1,30,5,30,553,8,30,10,30,12,30,
  	556,9,30,1,30,1,30,1,30,1,30,1,30,1,30,1,30,1,30,1,30,1,30,1,30,5,30,
  	569,8,30,10,30,12,30,572,9,30,1,30,1,30,1,30,1,30,1,30,1,30,1,30,3,30,
  	581,8,30,1,31,1,31,3,31,585,8,31,1,31,1,31,5,31,589,8,31,10,31,12,31,
  	592,9,31,1,31,1,31,1,32,1,32,1,32,1,32,3,32,600,8,32,1,32,1,32,1,33,1,
  	33,1,33,1,33,3,33,608,8,33,1,33,1,33,1,34,1,34,1,34,3,34,615,8,34,1,35,
  	1,35,1,36,1,36,4,36,621,8,36,11,36,12,36,622,1,36,5,36,626,8,36,10,36,
  	12,36,629,9,36,1,37,1,37,1,37,0,2,6,22,38,0,2,4,6,8,10,12,14,16,18,20,
  	22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,
  	68,70,72,74,0,3,1,0,24,25,1,0,27,28,9,0,5,6,11,11,15,16,18,18,20,22,27,
  	29,34,35,47,47,71,71,701,0,79,1,0,0,0,2,90,1,0,0,0,4,92,1,0,0,0,6,104,
  	1,0,0,0,8,115,1,0,0,0,10,127,1,0,0,0,12,146,1,0,0,0,14,148,1,0,0,0,16,
  	164,1,0,0,0,18,191,1,0,0,0,20,244,1,0,0,0,22,276,1,0,0,0,24,291,1,0,0,
  	0,26,337,1,0,0,0,28,339,1,0,0,0,30,387,1,0,0,0,32,389,1,0,0,0,34,413,
  	1,0,0,0,36,415,1,0,0,0,38,432,1,0,0,0,40,437,1,0,0,0,42,441,1,0,0,0,44,
  	456,1,0,0,0,46,476,1,0,0,0,48,478,1,0,0,0,50,505,1,0,0,0,52,524,1,0,0,
  	0,54,526,1,0,0,0,56,537,1,0,0,0,58,545,1,0,0,0,60,580,1,0,0,0,62,582,
  	1,0,0,0,64,595,1,0,0,0,66,603,1,0,0,0,68,611,1,0,0,0,70,616,1,0,0,0,72,
  	618,1,0,0,0,74,630,1,0,0,0,76,78,3,2,1,0,77,76,1,0,0,0,78,81,1,0,0,0,
  	79,77,1,0,0,0,79,80,1,0,0,0,80,82,1,0,0,0,81,79,1,0,0,0,82,83,5,0,0,1,
  	83,1,1,0,0,0,84,91,3,4,2,0,85,91,3,8,4,0,86,91,3,10,5,0,87,91,3,54,27,
  	0,88,91,3,58,29,0,89,91,3,62,31,0,90,84,1,0,0,0,90,85,1,0,0,0,90,86,1,
  	0,0,0,90,87,1,0,0,0,90,88,1,0,0,0,90,89,1,0,0,0,91,3,1,0,0,0,92,93,5,
  	8,0,0,93,99,5,56,0,0,94,95,3,6,3,0,95,96,5,55,0,0,96,98,1,0,0,0,97,94,
  	1,0,0,0,98,101,1,0,0,0,99,97,1,0,0,0,99,100,1,0,0,0,100,102,1,0,0,0,101,
  	99,1,0,0,0,102,103,5,57,0,0,103,5,1,0,0,0,104,105,6,3,-1,0,105,106,3,
  	14,7,0,106,112,1,0,0,0,107,108,10,2,0,0,108,109,5,53,0,0,109,111,3,14,
  	7,0,110,107,1,0,0,0,111,114,1,0,0,0,112,110,1,0,0,0,112,113,1,0,0,0,113,
  	7,1,0,0,0,114,112,1,0,0,0,115,116,3,12,6,0,116,117,5,7,0,0,117,118,3,
  	14,7,0,118,122,5,56,0,0,119,121,3,20,10,0,120,119,1,0,0,0,121,124,1,0,
  	0,0,122,120,1,0,0,0,122,123,1,0,0,0,123,125,1,0,0,0,124,122,1,0,0,0,125,
  	126,5,57,0,0,126,9,1,0,0,0,127,128,5,35,0,0,128,129,5,56,0,0,129,130,
  	5,37,0,0,130,131,5,62,0,0,131,132,5,63,0,0,132,137,3,72,36,0,133,134,
  	5,60,0,0,134,136,3,72,36,0,135,133,1,0,0,0,136,139,1,0,0,0,137,135,1,
  	0,0,0,137,138,1,0,0,0,138,141,1,0,0,0,139,137,1,0,0,0,140,142,5,60,0,
  	0,141,140,1,0,0,0,141,142,1,0,0,0,142,143,1,0,0,0,143,144,5,64,0,0,144,
  	145,5,57,0,0,145,11,1,0,0,0,146,147,3,72,36,0,147,13,1,0,0,0,148,154,
  	3,72,36,0,149,150,5,58,0,0,150,151,5,6,0,0,151,152,3,72,36,0,152,153,
  	5,59,0,0,153,155,1,0,0,0,154,149,1,0,0,0,154,155,1,0,0,0,155,15,1,0,0,
  	0,156,165,3,72,36,0,157,158,3,72,36,0,158,159,5,66,0,0,159,160,3,72,36,
  	0,160,165,1,0,0,0,161,162,3,72,36,0,162,163,3,72,36,0,163,165,1,0,0,0,
  	164,156,1,0,0,0,164,157,1,0,0,0,164,161,1,0,0,0,165,17,1,0,0,0,166,167,
  	5,16,0,0,167,170,3,22,11,0,168,169,5,53,0,0,169,171,3,30,15,0,170,168,
  	1,0,0,0,170,171,1,0,0,0,171,174,1,0,0,0,172,173,5,70,0,0,173,175,3,72,
  	36,0,174,172,1,0,0,0,174,175,1,0,0,0,175,178,1,0,0,0,176,177,5,5,0,0,
  	177,179,3,72,36,0,178,176,1,0,0,0,178,179,1,0,0,0,179,180,1,0,0,0,180,
  	181,5,55,0,0,181,192,1,0,0,0,182,183,5,11,0,0,183,184,3,22,11,0,184,185,
  	5,55,0,0,185,192,1,0,0,0,186,187,5,17,0,0,187,188,3,72,36,0,188,189,5,
  	55,0,0,189,192,1,0,0,0,190,192,3,66,33,0,191,166,1,0,0,0,191,182,1,0,
  	0,0,191,186,1,0,0,0,191,190,1,0,0,0,192,19,1,0,0,0,193,194,5,9,0,0,194,
  	245,5,55,0,0,195,196,5,10,0,0,196,197,3,72,36,0,197,198,5,55,0,0,198,
  	245,1,0,0,0,199,201,3,16,8,0,200,199,1,0,0,0,200,201,1,0,0,0,201,202,
  	1,0,0,0,202,203,5,11,0,0,203,204,3,22,11,0,204,205,5,55,0,0,205,245,1,
  	0,0,0,206,207,5,13,0,0,207,210,3,22,11,0,208,209,5,5,0,0,209,211,3,72,
  	36,0,210,208,1,0,0,0,210,211,1,0,0,0,211,212,1,0,0,0,212,213,5,55,0,0,
  	213,245,1,0,0,0,214,215,5,14,0,0,215,245,5,55,0,0,216,217,3,42,21,0,217,
  	218,5,55,0,0,218,245,1,0,0,0,219,220,5,15,0,0,220,221,3,22,11,0,221,222,
  	5,55,0,0,222,245,1,0,0,0,223,224,5,16,0,0,224,228,5,56,0,0,225,227,3,
  	18,9,0,226,225,1,0,0,0,227,230,1,0,0,0,228,226,1,0,0,0,228,229,1,0,0,
  	0,229,231,1,0,0,0,230,228,1,0,0,0,231,245,5,57,0,0,232,245,3,66,33,0,
  	233,234,5,20,0,0,234,235,5,72,0,0,235,245,5,55,0,0,236,237,5,21,0,0,237,
  	238,3,72,36,0,238,239,5,55,0,0,239,245,1,0,0,0,240,241,5,22,0,0,241,242,
  	3,40,20,0,242,243,5,55,0,0,243,245,1,0,0,0,244,193,1,0,0,0,244,195,1,
  	0,0,0,244,200,1,0,0,0,244,206,1,0,0,0,244,214,1,0,0,0,244,216,1,0,0,0,
  	244,219,1,0,0,0,244,223,1,0,0,0,244,232,1,0,0,0,244,233,1,0,0,0,244,236,
  	1,0,0,0,244,240,1,0,0,0,245,21,1,0,0,0,246,247,6,11,-1,0,247,248,3,72,
  	36,0,248,255,5,58,0,0,249,251,3,22,11,0,250,252,5,60,0,0,251,250,1,0,
  	0,0,251,252,1,0,0,0,252,254,1,0,0,0,253,249,1,0,0,0,254,257,1,0,0,0,255,
  	253,1,0,0,0,255,256,1,0,0,0,256,258,1,0,0,0,257,255,1,0,0,0,258,261,5,
  	59,0,0,259,260,5,53,0,0,260,262,3,30,15,0,261,259,1,0,0,0,261,262,1,0,
  	0,0,262,277,1,0,0,0,263,277,3,26,13,0,264,277,3,40,20,0,265,266,5,23,
  	0,0,266,277,3,14,7,0,267,268,3,24,12,0,268,269,5,26,0,0,269,270,5,23,
  	0,0,270,271,3,14,7,0,271,277,1,0,0,0,272,273,7,0,0,0,273,274,5,26,0,0,
  	274,275,5,23,0,0,275,277,3,14,7,0,276,246,1,0,0,0,276,263,1,0,0,0,276,
  	264,1,0,0,0,276,265,1,0,0,0,276,267,1,0,0,0,276,272,1,0,0,0,277,288,1,
  	0,0,0,278,279,10,5,0,0,279,280,5,4,0,0,280,287,3,30,15,0,281,282,10,1,
  	0,0,282,283,5,29,0,0,283,284,7,1,0,0,284,285,5,23,0,0,285,287,3,14,7,
  	0,286,278,1,0,0,0,286,281,1,0,0,0,287,290,1,0,0,0,288,286,1,0,0,0,288,
  	289,1,0,0,0,289,23,1,0,0,0,290,288,1,0,0,0,291,292,5,58,0,0,292,297,3,
  	22,11,0,293,294,5,60,0,0,294,296,3,22,11,0,295,293,1,0,0,0,296,299,1,
  	0,0,0,297,295,1,0,0,0,297,298,1,0,0,0,298,300,1,0,0,0,299,297,1,0,0,0,
  	300,301,5,59,0,0,301,25,1,0,0,0,302,305,5,72,0,0,303,304,5,65,0,0,304,
  	306,3,28,14,0,305,303,1,0,0,0,305,306,1,0,0,0,306,338,1,0,0,0,307,310,
  	5,73,0,0,308,309,5,65,0,0,309,311,3,28,14,0,310,308,1,0,0,0,310,311,1,
  	0,0,0,311,338,1,0,0,0,312,315,3,34,17,0,313,314,5,65,0,0,314,316,3,30,
  	15,0,315,313,1,0,0,0,315,316,1,0,0,0,316,338,1,0,0,0,317,320,3,38,19,
  	0,318,319,5,65,0,0,319,321,3,30,15,0,320,318,1,0,0,0,320,321,1,0,0,0,
  	321,338,1,0,0,0,322,325,5,47,0,0,323,324,5,65,0,0,324,326,3,30,15,0,325,
  	323,1,0,0,0,325,326,1,0,0,0,326,338,1,0,0,0,327,330,5,48,0,0,328,329,
  	5,65,0,0,329,331,3,28,14,0,330,328,1,0,0,0,330,331,1,0,0,0,331,338,1,
  	0,0,0,332,335,5,49,0,0,333,334,5,65,0,0,334,336,3,28,14,0,335,333,1,0,
  	0,0,335,336,1,0,0,0,336,338,1,0,0,0,337,302,1,0,0,0,337,307,1,0,0,0,337,
  	312,1,0,0,0,337,317,1,0,0,0,337,322,1,0,0,0,337,327,1,0,0,0,337,332,1,
  	0,0,0,338,27,1,0,0,0,339,341,3,72,36,0,340,342,5,69,0,0,341,340,1,0,0,
  	0,341,342,1,0,0,0,342,344,1,0,0,0,343,345,3,32,16,0,344,343,1,0,0,0,344,
  	345,1,0,0,0,345,29,1,0,0,0,346,388,3,28,14,0,347,349,5,50,0,0,348,350,
  	5,69,0,0,349,348,1,0,0,0,349,350,1,0,0,0,350,351,1,0,0,0,351,353,5,67,
  	0,0,352,354,3,30,15,0,353,352,1,0,0,0,353,354,1,0,0,0,354,355,1,0,0,0,
  	355,388,5,68,0,0,356,358,5,51,0,0,357,359,5,69,0,0,358,357,1,0,0,0,358,
  	359,1,0,0,0,359,360,1,0,0,0,360,362,5,67,0,0,361,363,3,28,14,0,362,361,
  	1,0,0,0,362,363,1,0,0,0,363,365,1,0,0,0,364,366,5,60,0,0,365,364,1,0,
  	0,0,365,366,1,0,0,0,366,368,1,0,0,0,367,369,3,30,15,0,368,367,1,0,0,0,
  	368,369,1,0,0,0,369,370,1,0,0,0,370,388,5,68,0,0,371,373,5,52,0,0,372,
  	374,5,69,0,0,373,372,1,0,0,0,373,374,1,0,0,0,374,375,1,0,0,0,375,377,
  	5,67,0,0,376,378,3,30,15,0,377,376,1,0,0,0,377,378,1,0,0,0,378,383,1,
  	0,0,0,379,380,5,60,0,0,380,382,3,30,15,0,381,379,1,0,0,0,382,385,1,0,
  	0,0,383,381,1,0,0,0,383,384,1,0,0,0,384,386,1,0,0,0,385,383,1,0,0,0,386,
  	388,5,68,0,0,387,346,1,0,0,0,387,347,1,0,0,0,387,356,1,0,0,0,387,371,
  	1,0,0,0,388,31,1,0,0,0,389,390,5,67,0,0,390,395,5,72,0,0,391,392,5,60,
  	0,0,392,394,5,72,0,0,393,391,1,0,0,0,394,397,1,0,0,0,395,393,1,0,0,0,
  	395,396,1,0,0,0,396,398,1,0,0,0,397,395,1,0,0,0,398,399,5,68,0,0,399,
  	33,1,0,0,0,400,401,5,56,0,0,401,406,3,36,18,0,402,403,5,60,0,0,403,405,
  	3,36,18,0,404,402,1,0,0,0,405,408,1,0,0,0,406,404,1,0,0,0,406,407,1,0,
  	0,0,407,409,1,0,0,0,408,406,1,0,0,0,409,410,5,57,0,0,410,414,1,0,0,0,
  	411,412,5,56,0,0,412,414,5,57,0,0,413,400,1,0,0,0,413,411,1,0,0,0,414,
  	35,1,0,0,0,415,416,3,26,13,0,416,417,5,54,0,0,417,418,3,26,13,0,418,37,
  	1,0,0,0,419,420,5,56,0,0,420,425,3,26,13,0,421,422,5,60,0,0,422,424,3,
  	26,13,0,423,421,1,0,0,0,424,427,1,0,0,0,425,423,1,0,0,0,425,426,1,0,0,
  	0,426,428,1,0,0,0,427,425,1,0,0,0,428,429,5,57,0,0,429,433,1,0,0,0,430,
  	431,5,56,0,0,431,433,5,57,0,0,432,419,1,0,0,0,432,430,1,0,0,0,433,39,
  	1,0,0,0,434,435,3,72,36,0,435,436,5,61,0,0,436,438,1,0,0,0,437,434,1,
  	0,0,0,437,438,1,0,0,0,438,439,1,0,0,0,439,440,3,72,36,0,440,41,1,0,0,
  	0,441,442,5,34,0,0,442,443,3,72,36,0,443,43,1,0,0,0,444,445,5,38,0,0,
  	445,446,5,54,0,0,446,457,5,73,0,0,447,448,5,39,0,0,448,449,5,54,0,0,449,
  	457,5,73,0,0,450,451,5,40,0,0,451,452,5,54,0,0,452,457,5,73,0,0,453,454,
  	5,41,0,0,454,455,5,54,0,0,455,457,5,73,0,0,456,444,1,0,0,0,456,447,1,
  	0,0,0,456,450,1,0,0,0,456,453,1,0,0,0,457,45,1,0,0,0,458,459,5,42,0,0,
  	459,460,5,54,0,0,460,477,5,72,0,0,461,462,5,43,0,0,462,463,5,54,0,0,463,
  	477,5,72,0,0,464,465,5,44,0,0,465,466,5,54,0,0,466,477,5,72,0,0,467,468,
  	5,45,0,0,468,469,5,54,0,0,469,470,5,56,0,0,470,477,5,57,0,0,471,472,5,
  	46,0,0,472,473,5,54,0,0,473,474,5,56,0,0,474,477,5,57,0,0,475,477,3,44,
  	22,0,476,458,1,0,0,0,476,461,1,0,0,0,476,464,1,0,0,0,476,467,1,0,0,0,
  	476,471,1,0,0,0,476,475,1,0,0,0,477,47,1,0,0,0,478,482,5,56,0,0,479,481,
  	3,46,23,0,480,479,1,0,0,0,481,484,1,0,0,0,482,480,1,0,0,0,482,483,1,0,
  	0,0,483,485,1,0,0,0,484,482,1,0,0,0,485,486,5,57,0,0,486,49,1,0,0,0,487,
  	488,5,14,0,0,488,506,3,72,36,0,489,490,5,36,0,0,490,491,5,62,0,0,491,
  	492,5,63,0,0,492,497,3,48,24,0,493,494,5,60,0,0,494,496,3,48,24,0,495,
  	493,1,0,0,0,496,499,1,0,0,0,497,495,1,0,0,0,497,498,1,0,0,0,498,501,1,
  	0,0,0,499,497,1,0,0,0,500,502,5,60,0,0,501,500,1,0,0,0,501,502,1,0,0,
  	0,502,503,1,0,0,0,503,504,5,64,0,0,504,506,1,0,0,0,505,487,1,0,0,0,505,
  	489,1,0,0,0,506,51,1,0,0,0,507,508,5,14,0,0,508,525,3,72,36,0,509,510,
  	5,37,0,0,510,511,5,62,0,0,511,512,5,63,0,0,512,517,5,73,0,0,513,514,5,
  	60,0,0,514,516,5,73,0,0,515,513,1,0,0,0,516,519,1,0,0,0,517,515,1,0,0,
  	0,517,518,1,0,0,0,518,521,1,0,0,0,519,517,1,0,0,0,520,522,5,60,0,0,521,
  	520,1,0,0,0,521,522,1,0,0,0,522,523,1,0,0,0,523,525,5,64,0,0,524,507,
  	1,0,0,0,524,509,1,0,0,0,525,53,1,0,0,0,526,527,5,6,0,0,527,528,3,72,36,
  	0,528,532,5,56,0,0,529,531,3,56,28,0,530,529,1,0,0,0,531,534,1,0,0,0,
  	532,530,1,0,0,0,532,533,1,0,0,0,533,535,1,0,0,0,534,532,1,0,0,0,535,536,
  	5,57,0,0,536,55,1,0,0,0,537,538,3,72,36,0,538,541,3,30,15,0,539,540,5,
  	5,0,0,540,542,3,72,36,0,541,539,1,0,0,0,541,542,1,0,0,0,542,543,1,0,0,
  	0,543,544,5,55,0,0,544,57,1,0,0,0,545,546,5,34,0,0,546,547,3,60,30,0,
  	547,59,1,0,0,0,548,549,5,31,0,0,549,550,3,72,36,0,550,554,5,56,0,0,551,
  	553,3,50,25,0,552,551,1,0,0,0,553,556,1,0,0,0,554,552,1,0,0,0,554,555,
  	1,0,0,0,555,557,1,0,0,0,556,554,1,0,0,0,557,558,5,57,0,0,558,581,1,0,
  	0,0,559,560,5,30,0,0,560,561,3,72,36,0,561,562,5,56,0,0,562,563,5,57,
  	0,0,563,581,1,0,0,0,564,565,5,32,0,0,565,566,3,72,36,0,566,570,5,56,0,
  	0,567,569,3,52,26,0,568,567,1,0,0,0,569,572,1,0,0,0,570,568,1,0,0,0,570,
  	571,1,0,0,0,571,573,1,0,0,0,572,570,1,0,0,0,573,574,5,57,0,0,574,581,
  	1,0,0,0,575,576,5,33,0,0,576,577,3,72,36,0,577,578,5,56,0,0,578,579,5,
  	57,0,0,579,581,1,0,0,0,580,548,1,0,0,0,580,559,1,0,0,0,580,564,1,0,0,
  	0,580,575,1,0,0,0,581,61,1,0,0,0,582,584,5,2,0,0,583,585,5,75,0,0,584,
  	583,1,0,0,0,584,585,1,0,0,0,585,586,1,0,0,0,586,590,5,56,0,0,587,589,
  	3,64,32,0,588,587,1,0,0,0,589,592,1,0,0,0,590,588,1,0,0,0,590,591,1,0,
  	0,0,591,593,1,0,0,0,592,590,1,0,0,0,593,594,5,57,0,0,594,63,1,0,0,0,595,
  	596,5,3,0,0,596,599,3,68,34,0,597,598,5,4,0,0,598,600,3,72,36,0,599,597,
  	1,0,0,0,599,600,1,0,0,0,600,601,1,0,0,0,601,602,5,55,0,0,602,65,1,0,0,
  	0,603,604,5,18,0,0,604,607,3,22,11,0,605,606,5,19,0,0,606,608,3,72,36,
  	0,607,605,1,0,0,0,607,608,1,0,0,0,608,609,1,0,0,0,609,610,5,55,0,0,610,
  	67,1,0,0,0,611,612,3,72,36,0,612,614,5,54,0,0,613,615,3,70,35,0,614,613,
  	1,0,0,0,614,615,1,0,0,0,615,69,1,0,0,0,616,617,3,72,36,0,617,71,1,0,0,
  	0,618,627,3,74,37,0,619,621,5,65,0,0,620,619,1,0,0,0,621,622,1,0,0,0,
  	622,620,1,0,0,0,622,623,1,0,0,0,623,624,1,0,0,0,624,626,3,74,37,0,625,
  	620,1,0,0,0,626,629,1,0,0,0,627,625,1,0,0,0,627,628,1,0,0,0,628,73,1,
  	0,0,0,629,627,1,0,0,0,630,631,7,2,0,0,631,75,1,0,0,0,71,79,90,99,112,
  	122,137,141,154,164,170,174,178,191,200,210,228,244,251,255,261,276,286,
  	288,297,305,310,315,320,325,330,335,337,341,344,349,353,358,362,365,368,
  	373,377,383,387,395,406,413,425,432,437,456,476,482,497,501,505,517,521,
  	524,532,541,554,570,580,584,590,599,607,614,622,627
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  substraitplanparserParserStaticData = std::move(staticData);
}

}

SubstraitPlanParser::SubstraitPlanParser(TokenStream *input) : SubstraitPlanParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

SubstraitPlanParser::SubstraitPlanParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  SubstraitPlanParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *substraitplanparserParserStaticData->atn, substraitplanparserParserStaticData->decisionToDFA, substraitplanparserParserStaticData->sharedContextCache, options);
}

SubstraitPlanParser::~SubstraitPlanParser() {
  delete _interpreter;
}

const atn::ATN& SubstraitPlanParser::getATN() const {
  return *substraitplanparserParserStaticData->atn;
}

std::string SubstraitPlanParser::getGrammarFileName() const {
  return "SubstraitPlanParser.g4";
}

const std::vector<std::string>& SubstraitPlanParser::getRuleNames() const {
  return substraitplanparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& SubstraitPlanParser::getVocabulary() const {
  return substraitplanparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView SubstraitPlanParser::getSerializedATN() const {
  return substraitplanparserParserStaticData->serializedATN;
}


//----------------- PlanContext ------------------------------------------------------------------

SubstraitPlanParser::PlanContext::PlanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::PlanContext::EOF() {
  return getToken(SubstraitPlanParser::EOF, 0);
}

std::vector<SubstraitPlanParser::Plan_detailContext *> SubstraitPlanParser::PlanContext::plan_detail() {
  return getRuleContexts<SubstraitPlanParser::Plan_detailContext>();
}

SubstraitPlanParser::Plan_detailContext* SubstraitPlanParser::PlanContext::plan_detail(size_t i) {
  return getRuleContext<SubstraitPlanParser::Plan_detailContext>(i);
}


size_t SubstraitPlanParser::PlanContext::getRuleIndex() const {
  return SubstraitPlanParser::RulePlan;
}


std::any SubstraitPlanParser::PlanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitPlan(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::PlanContext* SubstraitPlanParser::plan() {
  PlanContext *_localctx = _tracker.createInstance<PlanContext>(_ctx, getState());
  enterRule(_localctx, 0, SubstraitPlanParser::RulePlan);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(79);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 140789975189860) != 0) || _la == SubstraitPlanParser::IDENTIFIER) {
      setState(76);
      plan_detail();
      setState(81);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(82);
    match(SubstraitPlanParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Plan_detailContext ------------------------------------------------------------------

SubstraitPlanParser::Plan_detailContext::Plan_detailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SubstraitPlanParser::PipelinesContext* SubstraitPlanParser::Plan_detailContext::pipelines() {
  return getRuleContext<SubstraitPlanParser::PipelinesContext>(0);
}

SubstraitPlanParser::RelationContext* SubstraitPlanParser::Plan_detailContext::relation() {
  return getRuleContext<SubstraitPlanParser::RelationContext>(0);
}

SubstraitPlanParser::Root_relationContext* SubstraitPlanParser::Plan_detailContext::root_relation() {
  return getRuleContext<SubstraitPlanParser::Root_relationContext>(0);
}

SubstraitPlanParser::Schema_definitionContext* SubstraitPlanParser::Plan_detailContext::schema_definition() {
  return getRuleContext<SubstraitPlanParser::Schema_definitionContext>(0);
}

SubstraitPlanParser::Source_definitionContext* SubstraitPlanParser::Plan_detailContext::source_definition() {
  return getRuleContext<SubstraitPlanParser::Source_definitionContext>(0);
}

SubstraitPlanParser::ExtensionspaceContext* SubstraitPlanParser::Plan_detailContext::extensionspace() {
  return getRuleContext<SubstraitPlanParser::ExtensionspaceContext>(0);
}


size_t SubstraitPlanParser::Plan_detailContext::getRuleIndex() const {
  return SubstraitPlanParser::RulePlan_detail;
}


std::any SubstraitPlanParser::Plan_detailContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitPlan_detail(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Plan_detailContext* SubstraitPlanParser::plan_detail() {
  Plan_detailContext *_localctx = _tracker.createInstance<Plan_detailContext>(_ctx, getState());
  enterRule(_localctx, 2, SubstraitPlanParser::RulePlan_detail);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(90);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(84);
      pipelines();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(85);
      relation();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(86);
      root_relation();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(87);
      schema_definition();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(88);
      source_definition();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(89);
      extensionspace();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PipelinesContext ------------------------------------------------------------------

SubstraitPlanParser::PipelinesContext::PipelinesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::PipelinesContext::PIPELINES() {
  return getToken(SubstraitPlanParser::PIPELINES, 0);
}

tree::TerminalNode* SubstraitPlanParser::PipelinesContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::PipelinesContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

std::vector<SubstraitPlanParser::PipelineContext *> SubstraitPlanParser::PipelinesContext::pipeline() {
  return getRuleContexts<SubstraitPlanParser::PipelineContext>();
}

SubstraitPlanParser::PipelineContext* SubstraitPlanParser::PipelinesContext::pipeline(size_t i) {
  return getRuleContext<SubstraitPlanParser::PipelineContext>(i);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::PipelinesContext::SEMICOLON() {
  return getTokens(SubstraitPlanParser::SEMICOLON);
}

tree::TerminalNode* SubstraitPlanParser::PipelinesContext::SEMICOLON(size_t i) {
  return getToken(SubstraitPlanParser::SEMICOLON, i);
}


size_t SubstraitPlanParser::PipelinesContext::getRuleIndex() const {
  return SubstraitPlanParser::RulePipelines;
}


std::any SubstraitPlanParser::PipelinesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitPipelines(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::PipelinesContext* SubstraitPlanParser::pipelines() {
  PipelinesContext *_localctx = _tracker.createInstance<PipelinesContext>(_ctx, getState());
  enterRule(_localctx, 4, SubstraitPlanParser::RulePipelines);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(92);
    match(SubstraitPlanParser::PIPELINES);
    setState(93);
    match(SubstraitPlanParser::LEFTBRACE);
    setState(99);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 140789975189600) != 0) || _la == SubstraitPlanParser::IDENTIFIER) {
      setState(94);
      pipeline(0);
      setState(95);
      match(SubstraitPlanParser::SEMICOLON);
      setState(101);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(102);
    match(SubstraitPlanParser::RIGHTBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PipelineContext ------------------------------------------------------------------

SubstraitPlanParser::PipelineContext::PipelineContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SubstraitPlanParser::Relation_refContext* SubstraitPlanParser::PipelineContext::relation_ref() {
  return getRuleContext<SubstraitPlanParser::Relation_refContext>(0);
}

SubstraitPlanParser::PipelineContext* SubstraitPlanParser::PipelineContext::pipeline() {
  return getRuleContext<SubstraitPlanParser::PipelineContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::PipelineContext::ARROW() {
  return getToken(SubstraitPlanParser::ARROW, 0);
}


size_t SubstraitPlanParser::PipelineContext::getRuleIndex() const {
  return SubstraitPlanParser::RulePipeline;
}


std::any SubstraitPlanParser::PipelineContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitPipeline(this);
  else
    return visitor->visitChildren(this);
}


SubstraitPlanParser::PipelineContext* SubstraitPlanParser::pipeline() {
   return pipeline(0);
}

SubstraitPlanParser::PipelineContext* SubstraitPlanParser::pipeline(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SubstraitPlanParser::PipelineContext *_localctx = _tracker.createInstance<PipelineContext>(_ctx, parentState);
  SubstraitPlanParser::PipelineContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 6;
  enterRecursionRule(_localctx, 6, SubstraitPlanParser::RulePipeline, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(105);
    relation_ref();
    _ctx->stop = _input->LT(-1);
    setState(112);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<PipelineContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RulePipeline);
        setState(107);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(108);
        match(SubstraitPlanParser::ARROW);
        setState(109);
        relation_ref(); 
      }
      setState(114);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- RelationContext ------------------------------------------------------------------

SubstraitPlanParser::RelationContext::RelationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SubstraitPlanParser::Relation_typeContext* SubstraitPlanParser::RelationContext::relation_type() {
  return getRuleContext<SubstraitPlanParser::Relation_typeContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::RelationContext::RELATION() {
  return getToken(SubstraitPlanParser::RELATION, 0);
}

SubstraitPlanParser::Relation_refContext* SubstraitPlanParser::RelationContext::relation_ref() {
  return getRuleContext<SubstraitPlanParser::Relation_refContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::RelationContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::RelationContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

std::vector<SubstraitPlanParser::Relation_detailContext *> SubstraitPlanParser::RelationContext::relation_detail() {
  return getRuleContexts<SubstraitPlanParser::Relation_detailContext>();
}

SubstraitPlanParser::Relation_detailContext* SubstraitPlanParser::RelationContext::relation_detail(size_t i) {
  return getRuleContext<SubstraitPlanParser::Relation_detailContext>(i);
}


size_t SubstraitPlanParser::RelationContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleRelation;
}


std::any SubstraitPlanParser::RelationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelation(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::RelationContext* SubstraitPlanParser::relation() {
  RelationContext *_localctx = _tracker.createInstance<RelationContext>(_ctx, getState());
  enterRule(_localctx, 8, SubstraitPlanParser::RuleRelation);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(115);
    relation_type();
    setState(116);
    match(SubstraitPlanParser::RELATION);
    setState(117);
    relation_ref();
    setState(118);
    match(SubstraitPlanParser::LEFTBRACE);
    setState(122);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 140789975215712) != 0) || _la == SubstraitPlanParser::IDENTIFIER) {
      setState(119);
      relation_detail();
      setState(124);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(125);
    match(SubstraitPlanParser::RIGHTBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Root_relationContext ------------------------------------------------------------------

SubstraitPlanParser::Root_relationContext::Root_relationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Root_relationContext::ROOT() {
  return getToken(SubstraitPlanParser::ROOT, 0);
}

tree::TerminalNode* SubstraitPlanParser::Root_relationContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::Root_relationContext::NAMES() {
  return getToken(SubstraitPlanParser::NAMES, 0);
}

tree::TerminalNode* SubstraitPlanParser::Root_relationContext::EQUAL() {
  return getToken(SubstraitPlanParser::EQUAL, 0);
}

tree::TerminalNode* SubstraitPlanParser::Root_relationContext::LEFTBRACKET() {
  return getToken(SubstraitPlanParser::LEFTBRACKET, 0);
}

std::vector<SubstraitPlanParser::IdContext *> SubstraitPlanParser::Root_relationContext::id() {
  return getRuleContexts<SubstraitPlanParser::IdContext>();
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Root_relationContext::id(size_t i) {
  return getRuleContext<SubstraitPlanParser::IdContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Root_relationContext::RIGHTBRACKET() {
  return getToken(SubstraitPlanParser::RIGHTBRACKET, 0);
}

tree::TerminalNode* SubstraitPlanParser::Root_relationContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::Root_relationContext::COMMA() {
  return getTokens(SubstraitPlanParser::COMMA);
}

tree::TerminalNode* SubstraitPlanParser::Root_relationContext::COMMA(size_t i) {
  return getToken(SubstraitPlanParser::COMMA, i);
}


size_t SubstraitPlanParser::Root_relationContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleRoot_relation;
}


std::any SubstraitPlanParser::Root_relationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRoot_relation(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Root_relationContext* SubstraitPlanParser::root_relation() {
  Root_relationContext *_localctx = _tracker.createInstance<Root_relationContext>(_ctx, getState());
  enterRule(_localctx, 10, SubstraitPlanParser::RuleRoot_relation);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(127);
    match(SubstraitPlanParser::ROOT);
    setState(128);
    match(SubstraitPlanParser::LEFTBRACE);
    setState(129);
    match(SubstraitPlanParser::NAMES);
    setState(130);
    match(SubstraitPlanParser::EQUAL);
    setState(131);
    match(SubstraitPlanParser::LEFTBRACKET);
    setState(132);
    id();
    setState(137);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(133);
        match(SubstraitPlanParser::COMMA);
        setState(134);
        id(); 
      }
      setState(139);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    }
    setState(141);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SubstraitPlanParser::COMMA) {
      setState(140);
      match(SubstraitPlanParser::COMMA);
    }
    setState(143);
    match(SubstraitPlanParser::RIGHTBRACKET);
    setState(144);
    match(SubstraitPlanParser::RIGHTBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Relation_typeContext ------------------------------------------------------------------

SubstraitPlanParser::Relation_typeContext::Relation_typeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Relation_typeContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}


size_t SubstraitPlanParser::Relation_typeContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleRelation_type;
}


std::any SubstraitPlanParser::Relation_typeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelation_type(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Relation_typeContext* SubstraitPlanParser::relation_type() {
  Relation_typeContext *_localctx = _tracker.createInstance<Relation_typeContext>(_ctx, getState());
  enterRule(_localctx, 12, SubstraitPlanParser::RuleRelation_type);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(146);
    id();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Relation_refContext ------------------------------------------------------------------

SubstraitPlanParser::Relation_refContext::Relation_refContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SubstraitPlanParser::IdContext *> SubstraitPlanParser::Relation_refContext::id() {
  return getRuleContexts<SubstraitPlanParser::IdContext>();
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Relation_refContext::id(size_t i) {
  return getRuleContext<SubstraitPlanParser::IdContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Relation_refContext::LEFTPAREN() {
  return getToken(SubstraitPlanParser::LEFTPAREN, 0);
}

tree::TerminalNode* SubstraitPlanParser::Relation_refContext::SCHEMA() {
  return getToken(SubstraitPlanParser::SCHEMA, 0);
}

tree::TerminalNode* SubstraitPlanParser::Relation_refContext::RIGHTPAREN() {
  return getToken(SubstraitPlanParser::RIGHTPAREN, 0);
}


size_t SubstraitPlanParser::Relation_refContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleRelation_ref;
}


std::any SubstraitPlanParser::Relation_refContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelation_ref(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Relation_refContext* SubstraitPlanParser::relation_ref() {
  Relation_refContext *_localctx = _tracker.createInstance<Relation_refContext>(_ctx, getState());
  enterRule(_localctx, 14, SubstraitPlanParser::RuleRelation_ref);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(148);
    id();
    setState(154);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      setState(149);
      match(SubstraitPlanParser::LEFTPAREN);
      setState(150);
      match(SubstraitPlanParser::SCHEMA);
      setState(151);
      id();
      setState(152);
      match(SubstraitPlanParser::RIGHTPAREN);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Relation_filter_behaviorContext ------------------------------------------------------------------

SubstraitPlanParser::Relation_filter_behaviorContext::Relation_filter_behaviorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SubstraitPlanParser::IdContext *> SubstraitPlanParser::Relation_filter_behaviorContext::id() {
  return getRuleContexts<SubstraitPlanParser::IdContext>();
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Relation_filter_behaviorContext::id(size_t i) {
  return getRuleContext<SubstraitPlanParser::IdContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Relation_filter_behaviorContext::MINUS() {
  return getToken(SubstraitPlanParser::MINUS, 0);
}


size_t SubstraitPlanParser::Relation_filter_behaviorContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleRelation_filter_behavior;
}


std::any SubstraitPlanParser::Relation_filter_behaviorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelation_filter_behavior(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Relation_filter_behaviorContext* SubstraitPlanParser::relation_filter_behavior() {
  Relation_filter_behaviorContext *_localctx = _tracker.createInstance<Relation_filter_behaviorContext>(_ctx, getState());
  enterRule(_localctx, 16, SubstraitPlanParser::RuleRelation_filter_behavior);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(164);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(156);
      id();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(157);
      id();
      setState(158);
      match(SubstraitPlanParser::MINUS);
      setState(159);
      id();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(161);
      id();
      setState(162);
      id();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Measure_detailContext ------------------------------------------------------------------

SubstraitPlanParser::Measure_detailContext::Measure_detailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Measure_detailContext::MEASURE() {
  return getToken(SubstraitPlanParser::MEASURE, 0);
}

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::Measure_detailContext::expression() {
  return getRuleContext<SubstraitPlanParser::ExpressionContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::Measure_detailContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

tree::TerminalNode* SubstraitPlanParser::Measure_detailContext::ARROW() {
  return getToken(SubstraitPlanParser::ARROW, 0);
}

SubstraitPlanParser::Literal_complex_typeContext* SubstraitPlanParser::Measure_detailContext::literal_complex_type() {
  return getRuleContext<SubstraitPlanParser::Literal_complex_typeContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::Measure_detailContext::ATSIGN() {
  return getToken(SubstraitPlanParser::ATSIGN, 0);
}

std::vector<SubstraitPlanParser::IdContext *> SubstraitPlanParser::Measure_detailContext::id() {
  return getRuleContexts<SubstraitPlanParser::IdContext>();
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Measure_detailContext::id(size_t i) {
  return getRuleContext<SubstraitPlanParser::IdContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Measure_detailContext::NAMED() {
  return getToken(SubstraitPlanParser::NAMED, 0);
}

tree::TerminalNode* SubstraitPlanParser::Measure_detailContext::FILTER() {
  return getToken(SubstraitPlanParser::FILTER, 0);
}

tree::TerminalNode* SubstraitPlanParser::Measure_detailContext::INVOCATION() {
  return getToken(SubstraitPlanParser::INVOCATION, 0);
}

SubstraitPlanParser::Sort_fieldContext* SubstraitPlanParser::Measure_detailContext::sort_field() {
  return getRuleContext<SubstraitPlanParser::Sort_fieldContext>(0);
}


size_t SubstraitPlanParser::Measure_detailContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleMeasure_detail;
}


std::any SubstraitPlanParser::Measure_detailContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitMeasure_detail(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Measure_detailContext* SubstraitPlanParser::measure_detail() {
  Measure_detailContext *_localctx = _tracker.createInstance<Measure_detailContext>(_ctx, getState());
  enterRule(_localctx, 18, SubstraitPlanParser::RuleMeasure_detail);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(191);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SubstraitPlanParser::MEASURE: {
        enterOuterAlt(_localctx, 1);
        setState(166);
        match(SubstraitPlanParser::MEASURE);
        setState(167);
        expression(0);
        setState(170);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SubstraitPlanParser::ARROW) {
          setState(168);
          match(SubstraitPlanParser::ARROW);
          setState(169);
          literal_complex_type();
        }
        setState(174);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SubstraitPlanParser::ATSIGN) {
          setState(172);
          match(SubstraitPlanParser::ATSIGN);
          setState(173);
          id();
        }
        setState(178);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SubstraitPlanParser::NAMED) {
          setState(176);
          match(SubstraitPlanParser::NAMED);
          setState(177);
          id();
        }
        setState(180);
        match(SubstraitPlanParser::SEMICOLON);
        break;
      }

      case SubstraitPlanParser::FILTER: {
        enterOuterAlt(_localctx, 2);
        setState(182);
        match(SubstraitPlanParser::FILTER);
        setState(183);
        expression(0);
        setState(184);
        match(SubstraitPlanParser::SEMICOLON);
        break;
      }

      case SubstraitPlanParser::INVOCATION: {
        enterOuterAlt(_localctx, 3);
        setState(186);
        match(SubstraitPlanParser::INVOCATION);
        setState(187);
        id();
        setState(188);
        match(SubstraitPlanParser::SEMICOLON);
        break;
      }

      case SubstraitPlanParser::SORT: {
        enterOuterAlt(_localctx, 4);
        setState(190);
        sort_field();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Relation_detailContext ------------------------------------------------------------------

SubstraitPlanParser::Relation_detailContext::Relation_detailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SubstraitPlanParser::Relation_detailContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleRelation_detail;
}

void SubstraitPlanParser::Relation_detailContext::copyFrom(Relation_detailContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- RelationSourceReferenceContext ------------------------------------------------------------------

SubstraitPlanParser::Source_referenceContext* SubstraitPlanParser::RelationSourceReferenceContext::source_reference() {
  return getRuleContext<SubstraitPlanParser::Source_referenceContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::RelationSourceReferenceContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

SubstraitPlanParser::RelationSourceReferenceContext::RelationSourceReferenceContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationSourceReferenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationSourceReference(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationEmitContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::RelationEmitContext::EMIT() {
  return getToken(SubstraitPlanParser::EMIT, 0);
}

SubstraitPlanParser::Column_nameContext* SubstraitPlanParser::RelationEmitContext::column_name() {
  return getRuleContext<SubstraitPlanParser::Column_nameContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::RelationEmitContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

SubstraitPlanParser::RelationEmitContext::RelationEmitContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationEmitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationEmit(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationFilterContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::RelationFilterContext::FILTER() {
  return getToken(SubstraitPlanParser::FILTER, 0);
}

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::RelationFilterContext::expression() {
  return getRuleContext<SubstraitPlanParser::ExpressionContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::RelationFilterContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

SubstraitPlanParser::Relation_filter_behaviorContext* SubstraitPlanParser::RelationFilterContext::relation_filter_behavior() {
  return getRuleContext<SubstraitPlanParser::Relation_filter_behaviorContext>(0);
}

SubstraitPlanParser::RelationFilterContext::RelationFilterContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationFilterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationFilter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationMeasureContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::RelationMeasureContext::MEASURE() {
  return getToken(SubstraitPlanParser::MEASURE, 0);
}

tree::TerminalNode* SubstraitPlanParser::RelationMeasureContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::RelationMeasureContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

std::vector<SubstraitPlanParser::Measure_detailContext *> SubstraitPlanParser::RelationMeasureContext::measure_detail() {
  return getRuleContexts<SubstraitPlanParser::Measure_detailContext>();
}

SubstraitPlanParser::Measure_detailContext* SubstraitPlanParser::RelationMeasureContext::measure_detail(size_t i) {
  return getRuleContext<SubstraitPlanParser::Measure_detailContext>(i);
}

SubstraitPlanParser::RelationMeasureContext::RelationMeasureContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationMeasureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationMeasure(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationUsesSchemaContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::RelationUsesSchemaContext::BASE_SCHEMA() {
  return getToken(SubstraitPlanParser::BASE_SCHEMA, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::RelationUsesSchemaContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::RelationUsesSchemaContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

SubstraitPlanParser::RelationUsesSchemaContext::RelationUsesSchemaContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationUsesSchemaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationUsesSchema(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationJoinTypeContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::RelationJoinTypeContext::TYPE() {
  return getToken(SubstraitPlanParser::TYPE, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::RelationJoinTypeContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::RelationJoinTypeContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

SubstraitPlanParser::RelationJoinTypeContext::RelationJoinTypeContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationJoinTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationJoinType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationAdvancedExtensionContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::RelationAdvancedExtensionContext::ADVANCED_EXTENSION() {
  return getToken(SubstraitPlanParser::ADVANCED_EXTENSION, 0);
}

tree::TerminalNode* SubstraitPlanParser::RelationAdvancedExtensionContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

SubstraitPlanParser::RelationAdvancedExtensionContext::RelationAdvancedExtensionContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationAdvancedExtensionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationAdvancedExtension(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationExpressionContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::RelationExpressionContext::EXPRESSION() {
  return getToken(SubstraitPlanParser::EXPRESSION, 0);
}

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::RelationExpressionContext::expression() {
  return getRuleContext<SubstraitPlanParser::ExpressionContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::RelationExpressionContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

tree::TerminalNode* SubstraitPlanParser::RelationExpressionContext::NAMED() {
  return getToken(SubstraitPlanParser::NAMED, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::RelationExpressionContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

SubstraitPlanParser::RelationExpressionContext::RelationExpressionContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationExpression(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationCountContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::RelationCountContext::COUNT() {
  return getToken(SubstraitPlanParser::COUNT, 0);
}

tree::TerminalNode* SubstraitPlanParser::RelationCountContext::NUMBER() {
  return getToken(SubstraitPlanParser::NUMBER, 0);
}

tree::TerminalNode* SubstraitPlanParser::RelationCountContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

SubstraitPlanParser::RelationCountContext::RelationCountContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationCountContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationCount(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationCommonContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::RelationCommonContext::COMMON() {
  return getToken(SubstraitPlanParser::COMMON, 0);
}

tree::TerminalNode* SubstraitPlanParser::RelationCommonContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

SubstraitPlanParser::RelationCommonContext::RelationCommonContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationCommonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationCommon(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationSortContext ------------------------------------------------------------------

SubstraitPlanParser::Sort_fieldContext* SubstraitPlanParser::RelationSortContext::sort_field() {
  return getRuleContext<SubstraitPlanParser::Sort_fieldContext>(0);
}

SubstraitPlanParser::RelationSortContext::RelationSortContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationSortContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationSort(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RelationGroupingContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::RelationGroupingContext::GROUPING() {
  return getToken(SubstraitPlanParser::GROUPING, 0);
}

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::RelationGroupingContext::expression() {
  return getRuleContext<SubstraitPlanParser::ExpressionContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::RelationGroupingContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

SubstraitPlanParser::RelationGroupingContext::RelationGroupingContext(Relation_detailContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::RelationGroupingContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitRelationGrouping(this);
  else
    return visitor->visitChildren(this);
}
SubstraitPlanParser::Relation_detailContext* SubstraitPlanParser::relation_detail() {
  Relation_detailContext *_localctx = _tracker.createInstance<Relation_detailContext>(_ctx, getState());
  enterRule(_localctx, 20, SubstraitPlanParser::RuleRelation_detail);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(244);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationCommonContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(193);
      match(SubstraitPlanParser::COMMON);
      setState(194);
      match(SubstraitPlanParser::SEMICOLON);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationUsesSchemaContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(195);
      match(SubstraitPlanParser::BASE_SCHEMA);
      setState(196);
      id();
      setState(197);
      match(SubstraitPlanParser::SEMICOLON);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationFilterContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(200);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
      case 1: {
        setState(199);
        relation_filter_behavior();
        break;
      }

      default:
        break;
      }
      setState(202);
      match(SubstraitPlanParser::FILTER);
      setState(203);
      expression(0);
      setState(204);
      match(SubstraitPlanParser::SEMICOLON);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationExpressionContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(206);
      match(SubstraitPlanParser::EXPRESSION);
      setState(207);
      expression(0);
      setState(210);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == SubstraitPlanParser::NAMED) {
        setState(208);
        match(SubstraitPlanParser::NAMED);
        setState(209);
        id();
      }
      setState(212);
      match(SubstraitPlanParser::SEMICOLON);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationAdvancedExtensionContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(214);
      match(SubstraitPlanParser::ADVANCED_EXTENSION);
      setState(215);
      match(SubstraitPlanParser::SEMICOLON);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationSourceReferenceContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(216);
      source_reference();
      setState(217);
      match(SubstraitPlanParser::SEMICOLON);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationGroupingContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(219);
      match(SubstraitPlanParser::GROUPING);
      setState(220);
      expression(0);
      setState(221);
      match(SubstraitPlanParser::SEMICOLON);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationMeasureContext>(_localctx);
      enterOuterAlt(_localctx, 8);
      setState(223);
      match(SubstraitPlanParser::MEASURE);
      setState(224);
      match(SubstraitPlanParser::LEFTBRACE);
      setState(228);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 460800) != 0)) {
        setState(225);
        measure_detail();
        setState(230);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(231);
      match(SubstraitPlanParser::RIGHTBRACE);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationSortContext>(_localctx);
      enterOuterAlt(_localctx, 9);
      setState(232);
      sort_field();
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationCountContext>(_localctx);
      enterOuterAlt(_localctx, 10);
      setState(233);
      match(SubstraitPlanParser::COUNT);
      setState(234);
      match(SubstraitPlanParser::NUMBER);
      setState(235);
      match(SubstraitPlanParser::SEMICOLON);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationJoinTypeContext>(_localctx);
      enterOuterAlt(_localctx, 11);
      setState(236);
      match(SubstraitPlanParser::TYPE);
      setState(237);
      id();
      setState(238);
      match(SubstraitPlanParser::SEMICOLON);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<SubstraitPlanParser::RelationEmitContext>(_localctx);
      enterOuterAlt(_localctx, 12);
      setState(240);
      match(SubstraitPlanParser::EMIT);
      setState(241);
      column_name();
      setState(242);
      match(SubstraitPlanParser::SEMICOLON);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

SubstraitPlanParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SubstraitPlanParser::ExpressionContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleExpression;
}

void SubstraitPlanParser::ExpressionContext::copyFrom(ExpressionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ExpressionScalarSubqueryContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::ExpressionScalarSubqueryContext::SUBQUERY() {
  return getToken(SubstraitPlanParser::SUBQUERY, 0);
}

SubstraitPlanParser::Relation_refContext* SubstraitPlanParser::ExpressionScalarSubqueryContext::relation_ref() {
  return getRuleContext<SubstraitPlanParser::Relation_refContext>(0);
}

SubstraitPlanParser::ExpressionScalarSubqueryContext::ExpressionScalarSubqueryContext(ExpressionContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::ExpressionScalarSubqueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExpressionScalarSubquery(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpressionConstantContext ------------------------------------------------------------------

SubstraitPlanParser::ConstantContext* SubstraitPlanParser::ExpressionConstantContext::constant() {
  return getRuleContext<SubstraitPlanParser::ConstantContext>(0);
}

SubstraitPlanParser::ExpressionConstantContext::ExpressionConstantContext(ExpressionContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::ExpressionConstantContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExpressionConstant(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpressionFunctionUseContext ------------------------------------------------------------------

SubstraitPlanParser::IdContext* SubstraitPlanParser::ExpressionFunctionUseContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionFunctionUseContext::LEFTPAREN() {
  return getToken(SubstraitPlanParser::LEFTPAREN, 0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionFunctionUseContext::RIGHTPAREN() {
  return getToken(SubstraitPlanParser::RIGHTPAREN, 0);
}

std::vector<SubstraitPlanParser::ExpressionContext *> SubstraitPlanParser::ExpressionFunctionUseContext::expression() {
  return getRuleContexts<SubstraitPlanParser::ExpressionContext>();
}

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::ExpressionFunctionUseContext::expression(size_t i) {
  return getRuleContext<SubstraitPlanParser::ExpressionContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionFunctionUseContext::ARROW() {
  return getToken(SubstraitPlanParser::ARROW, 0);
}

SubstraitPlanParser::Literal_complex_typeContext* SubstraitPlanParser::ExpressionFunctionUseContext::literal_complex_type() {
  return getRuleContext<SubstraitPlanParser::Literal_complex_typeContext>(0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::ExpressionFunctionUseContext::COMMA() {
  return getTokens(SubstraitPlanParser::COMMA);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionFunctionUseContext::COMMA(size_t i) {
  return getToken(SubstraitPlanParser::COMMA, i);
}

SubstraitPlanParser::ExpressionFunctionUseContext::ExpressionFunctionUseContext(ExpressionContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::ExpressionFunctionUseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExpressionFunctionUse(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpressionColumnContext ------------------------------------------------------------------

SubstraitPlanParser::Column_nameContext* SubstraitPlanParser::ExpressionColumnContext::column_name() {
  return getRuleContext<SubstraitPlanParser::Column_nameContext>(0);
}

SubstraitPlanParser::ExpressionColumnContext::ExpressionColumnContext(ExpressionContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::ExpressionColumnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExpressionColumn(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpressionSetComparisonSubqueryContext ------------------------------------------------------------------

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::ExpressionSetComparisonSubqueryContext::expression() {
  return getRuleContext<SubstraitPlanParser::ExpressionContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionSetComparisonSubqueryContext::COMPARISON() {
  return getToken(SubstraitPlanParser::COMPARISON, 0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionSetComparisonSubqueryContext::SUBQUERY() {
  return getToken(SubstraitPlanParser::SUBQUERY, 0);
}

SubstraitPlanParser::Relation_refContext* SubstraitPlanParser::ExpressionSetComparisonSubqueryContext::relation_ref() {
  return getRuleContext<SubstraitPlanParser::Relation_refContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionSetComparisonSubqueryContext::ALL() {
  return getToken(SubstraitPlanParser::ALL, 0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionSetComparisonSubqueryContext::ANY() {
  return getToken(SubstraitPlanParser::ANY, 0);
}

SubstraitPlanParser::ExpressionSetComparisonSubqueryContext::ExpressionSetComparisonSubqueryContext(ExpressionContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::ExpressionSetComparisonSubqueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExpressionSetComparisonSubquery(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpressionInPredicateSubqueryContext ------------------------------------------------------------------

SubstraitPlanParser::Expression_listContext* SubstraitPlanParser::ExpressionInPredicateSubqueryContext::expression_list() {
  return getRuleContext<SubstraitPlanParser::Expression_listContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionInPredicateSubqueryContext::IN() {
  return getToken(SubstraitPlanParser::IN, 0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionInPredicateSubqueryContext::SUBQUERY() {
  return getToken(SubstraitPlanParser::SUBQUERY, 0);
}

SubstraitPlanParser::Relation_refContext* SubstraitPlanParser::ExpressionInPredicateSubqueryContext::relation_ref() {
  return getRuleContext<SubstraitPlanParser::Relation_refContext>(0);
}

SubstraitPlanParser::ExpressionInPredicateSubqueryContext::ExpressionInPredicateSubqueryContext(ExpressionContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::ExpressionInPredicateSubqueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExpressionInPredicateSubquery(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpressionCastContext ------------------------------------------------------------------

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::ExpressionCastContext::expression() {
  return getRuleContext<SubstraitPlanParser::ExpressionContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionCastContext::AS() {
  return getToken(SubstraitPlanParser::AS, 0);
}

SubstraitPlanParser::Literal_complex_typeContext* SubstraitPlanParser::ExpressionCastContext::literal_complex_type() {
  return getRuleContext<SubstraitPlanParser::Literal_complex_typeContext>(0);
}

SubstraitPlanParser::ExpressionCastContext::ExpressionCastContext(ExpressionContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::ExpressionCastContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExpressionCast(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpressionSetPredicateSubqueryContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::ExpressionSetPredicateSubqueryContext::IN() {
  return getToken(SubstraitPlanParser::IN, 0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionSetPredicateSubqueryContext::SUBQUERY() {
  return getToken(SubstraitPlanParser::SUBQUERY, 0);
}

SubstraitPlanParser::Relation_refContext* SubstraitPlanParser::ExpressionSetPredicateSubqueryContext::relation_ref() {
  return getRuleContext<SubstraitPlanParser::Relation_refContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionSetPredicateSubqueryContext::UNIQUE() {
  return getToken(SubstraitPlanParser::UNIQUE, 0);
}

tree::TerminalNode* SubstraitPlanParser::ExpressionSetPredicateSubqueryContext::EXISTS() {
  return getToken(SubstraitPlanParser::EXISTS, 0);
}

SubstraitPlanParser::ExpressionSetPredicateSubqueryContext::ExpressionSetPredicateSubqueryContext(ExpressionContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::ExpressionSetPredicateSubqueryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExpressionSetPredicateSubquery(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::expression() {
   return expression(0);
}

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::expression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SubstraitPlanParser::ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, parentState);
  SubstraitPlanParser::ExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 22;
  enterRecursionRule(_localctx, 22, SubstraitPlanParser::RuleExpression, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(276);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ExpressionFunctionUseContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(247);
      id();
      setState(248);
      match(SubstraitPlanParser::LEFTPAREN);
      setState(255);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 361273185153681504) != 0) || ((((_la - 71) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 71)) & 7) != 0)) {
        setState(249);
        expression(0);
        setState(251);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SubstraitPlanParser::COMMA) {
          setState(250);
          match(SubstraitPlanParser::COMMA);
        }
        setState(257);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(258);
      match(SubstraitPlanParser::RIGHTPAREN);
      setState(261);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
      case 1: {
        setState(259);
        match(SubstraitPlanParser::ARROW);
        setState(260);
        literal_complex_type();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<ExpressionConstantContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(263);
      constant();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<ExpressionColumnContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(264);
      column_name();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<ExpressionScalarSubqueryContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(265);
      match(SubstraitPlanParser::SUBQUERY);
      setState(266);
      relation_ref();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<ExpressionInPredicateSubqueryContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(267);
      expression_list();
      setState(268);
      match(SubstraitPlanParser::IN);
      setState(269);
      match(SubstraitPlanParser::SUBQUERY);
      setState(270);
      relation_ref();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<ExpressionSetPredicateSubqueryContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(272);
      _la = _input->LA(1);
      if (!(_la == SubstraitPlanParser::EXISTS

      || _la == SubstraitPlanParser::UNIQUE)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(273);
      match(SubstraitPlanParser::IN);
      setState(274);
      match(SubstraitPlanParser::SUBQUERY);
      setState(275);
      relation_ref();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(288);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(286);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<ExpressionCastContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(278);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(279);
          match(SubstraitPlanParser::AS);
          setState(280);
          literal_complex_type();
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<ExpressionSetComparisonSubqueryContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(281);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(282);
          match(SubstraitPlanParser::COMPARISON);
          setState(283);
          _la = _input->LA(1);
          if (!(_la == SubstraitPlanParser::ALL

          || _la == SubstraitPlanParser::ANY)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(284);
          match(SubstraitPlanParser::SUBQUERY);
          setState(285);
          relation_ref();
          break;
        }

        default:
          break;
        } 
      }
      setState(290);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- Expression_listContext ------------------------------------------------------------------

SubstraitPlanParser::Expression_listContext::Expression_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Expression_listContext::LEFTPAREN() {
  return getToken(SubstraitPlanParser::LEFTPAREN, 0);
}

std::vector<SubstraitPlanParser::ExpressionContext *> SubstraitPlanParser::Expression_listContext::expression() {
  return getRuleContexts<SubstraitPlanParser::ExpressionContext>();
}

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::Expression_listContext::expression(size_t i) {
  return getRuleContext<SubstraitPlanParser::ExpressionContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Expression_listContext::RIGHTPAREN() {
  return getToken(SubstraitPlanParser::RIGHTPAREN, 0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::Expression_listContext::COMMA() {
  return getTokens(SubstraitPlanParser::COMMA);
}

tree::TerminalNode* SubstraitPlanParser::Expression_listContext::COMMA(size_t i) {
  return getToken(SubstraitPlanParser::COMMA, i);
}


size_t SubstraitPlanParser::Expression_listContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleExpression_list;
}


std::any SubstraitPlanParser::Expression_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExpression_list(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Expression_listContext* SubstraitPlanParser::expression_list() {
  Expression_listContext *_localctx = _tracker.createInstance<Expression_listContext>(_ctx, getState());
  enterRule(_localctx, 24, SubstraitPlanParser::RuleExpression_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(291);
    match(SubstraitPlanParser::LEFTPAREN);
    setState(292);
    expression(0);
    setState(297);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SubstraitPlanParser::COMMA) {
      setState(293);
      match(SubstraitPlanParser::COMMA);
      setState(294);
      expression(0);
      setState(299);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(300);
    match(SubstraitPlanParser::RIGHTPAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstantContext ------------------------------------------------------------------

SubstraitPlanParser::ConstantContext::ConstantContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::ConstantContext::NUMBER() {
  return getToken(SubstraitPlanParser::NUMBER, 0);
}

tree::TerminalNode* SubstraitPlanParser::ConstantContext::UNDERSCORE() {
  return getToken(SubstraitPlanParser::UNDERSCORE, 0);
}

SubstraitPlanParser::Literal_basic_typeContext* SubstraitPlanParser::ConstantContext::literal_basic_type() {
  return getRuleContext<SubstraitPlanParser::Literal_basic_typeContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::ConstantContext::STRING() {
  return getToken(SubstraitPlanParser::STRING, 0);
}

SubstraitPlanParser::Map_literalContext* SubstraitPlanParser::ConstantContext::map_literal() {
  return getRuleContext<SubstraitPlanParser::Map_literalContext>(0);
}

SubstraitPlanParser::Literal_complex_typeContext* SubstraitPlanParser::ConstantContext::literal_complex_type() {
  return getRuleContext<SubstraitPlanParser::Literal_complex_typeContext>(0);
}

SubstraitPlanParser::Struct_literalContext* SubstraitPlanParser::ConstantContext::struct_literal() {
  return getRuleContext<SubstraitPlanParser::Struct_literalContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::ConstantContext::NULLVAL() {
  return getToken(SubstraitPlanParser::NULLVAL, 0);
}

tree::TerminalNode* SubstraitPlanParser::ConstantContext::TRUEVAL() {
  return getToken(SubstraitPlanParser::TRUEVAL, 0);
}

tree::TerminalNode* SubstraitPlanParser::ConstantContext::FALSEVAL() {
  return getToken(SubstraitPlanParser::FALSEVAL, 0);
}


size_t SubstraitPlanParser::ConstantContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleConstant;
}


std::any SubstraitPlanParser::ConstantContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitConstant(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::ConstantContext* SubstraitPlanParser::constant() {
  ConstantContext *_localctx = _tracker.createInstance<ConstantContext>(_ctx, getState());
  enterRule(_localctx, 26, SubstraitPlanParser::RuleConstant);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(337);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(302);
      match(SubstraitPlanParser::NUMBER);
      setState(305);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
      case 1: {
        setState(303);
        match(SubstraitPlanParser::UNDERSCORE);
        setState(304);
        literal_basic_type();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(307);
      match(SubstraitPlanParser::STRING);
      setState(310);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
      case 1: {
        setState(308);
        match(SubstraitPlanParser::UNDERSCORE);
        setState(309);
        literal_basic_type();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(312);
      map_literal();
      setState(315);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
      case 1: {
        setState(313);
        match(SubstraitPlanParser::UNDERSCORE);
        setState(314);
        literal_complex_type();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(317);
      struct_literal();
      setState(320);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
      case 1: {
        setState(318);
        match(SubstraitPlanParser::UNDERSCORE);
        setState(319);
        literal_complex_type();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(322);
      match(SubstraitPlanParser::NULLVAL);
      setState(325);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
      case 1: {
        setState(323);
        match(SubstraitPlanParser::UNDERSCORE);
        setState(324);
        literal_complex_type();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(327);
      match(SubstraitPlanParser::TRUEVAL);
      setState(330);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
      case 1: {
        setState(328);
        match(SubstraitPlanParser::UNDERSCORE);
        setState(329);
        literal_basic_type();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(332);
      match(SubstraitPlanParser::FALSEVAL);
      setState(335);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
      case 1: {
        setState(333);
        match(SubstraitPlanParser::UNDERSCORE);
        setState(334);
        literal_basic_type();
        break;
      }

      default:
        break;
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Literal_basic_typeContext ------------------------------------------------------------------

SubstraitPlanParser::Literal_basic_typeContext::Literal_basic_typeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Literal_basic_typeContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::Literal_basic_typeContext::QUESTIONMARK() {
  return getToken(SubstraitPlanParser::QUESTIONMARK, 0);
}

SubstraitPlanParser::Literal_specifierContext* SubstraitPlanParser::Literal_basic_typeContext::literal_specifier() {
  return getRuleContext<SubstraitPlanParser::Literal_specifierContext>(0);
}


size_t SubstraitPlanParser::Literal_basic_typeContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleLiteral_basic_type;
}


std::any SubstraitPlanParser::Literal_basic_typeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitLiteral_basic_type(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Literal_basic_typeContext* SubstraitPlanParser::literal_basic_type() {
  Literal_basic_typeContext *_localctx = _tracker.createInstance<Literal_basic_typeContext>(_ctx, getState());
  enterRule(_localctx, 28, SubstraitPlanParser::RuleLiteral_basic_type);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(339);
    id();
    setState(341);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
    case 1: {
      setState(340);
      match(SubstraitPlanParser::QUESTIONMARK);
      break;
    }

    default:
      break;
    }
    setState(344);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx)) {
    case 1: {
      setState(343);
      literal_specifier();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Literal_complex_typeContext ------------------------------------------------------------------

SubstraitPlanParser::Literal_complex_typeContext::Literal_complex_typeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SubstraitPlanParser::Literal_basic_typeContext* SubstraitPlanParser::Literal_complex_typeContext::literal_basic_type() {
  return getRuleContext<SubstraitPlanParser::Literal_basic_typeContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::Literal_complex_typeContext::LIST() {
  return getToken(SubstraitPlanParser::LIST, 0);
}

tree::TerminalNode* SubstraitPlanParser::Literal_complex_typeContext::LEFTANGLEBRACKET() {
  return getToken(SubstraitPlanParser::LEFTANGLEBRACKET, 0);
}

tree::TerminalNode* SubstraitPlanParser::Literal_complex_typeContext::RIGHTANGLEBRACKET() {
  return getToken(SubstraitPlanParser::RIGHTANGLEBRACKET, 0);
}

tree::TerminalNode* SubstraitPlanParser::Literal_complex_typeContext::QUESTIONMARK() {
  return getToken(SubstraitPlanParser::QUESTIONMARK, 0);
}

std::vector<SubstraitPlanParser::Literal_complex_typeContext *> SubstraitPlanParser::Literal_complex_typeContext::literal_complex_type() {
  return getRuleContexts<SubstraitPlanParser::Literal_complex_typeContext>();
}

SubstraitPlanParser::Literal_complex_typeContext* SubstraitPlanParser::Literal_complex_typeContext::literal_complex_type(size_t i) {
  return getRuleContext<SubstraitPlanParser::Literal_complex_typeContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Literal_complex_typeContext::MAP() {
  return getToken(SubstraitPlanParser::MAP, 0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::Literal_complex_typeContext::COMMA() {
  return getTokens(SubstraitPlanParser::COMMA);
}

tree::TerminalNode* SubstraitPlanParser::Literal_complex_typeContext::COMMA(size_t i) {
  return getToken(SubstraitPlanParser::COMMA, i);
}

tree::TerminalNode* SubstraitPlanParser::Literal_complex_typeContext::STRUCT() {
  return getToken(SubstraitPlanParser::STRUCT, 0);
}


size_t SubstraitPlanParser::Literal_complex_typeContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleLiteral_complex_type;
}


std::any SubstraitPlanParser::Literal_complex_typeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitLiteral_complex_type(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Literal_complex_typeContext* SubstraitPlanParser::literal_complex_type() {
  Literal_complex_typeContext *_localctx = _tracker.createInstance<Literal_complex_typeContext>(_ctx, getState());
  enterRule(_localctx, 30, SubstraitPlanParser::RuleLiteral_complex_type);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(387);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SubstraitPlanParser::NAMED:
      case SubstraitPlanParser::SCHEMA:
      case SubstraitPlanParser::FILTER:
      case SubstraitPlanParser::GROUPING:
      case SubstraitPlanParser::MEASURE:
      case SubstraitPlanParser::SORT:
      case SubstraitPlanParser::COUNT:
      case SubstraitPlanParser::TYPE:
      case SubstraitPlanParser::EMIT:
      case SubstraitPlanParser::ALL:
      case SubstraitPlanParser::ANY:
      case SubstraitPlanParser::COMPARISON:
      case SubstraitPlanParser::SOURCE:
      case SubstraitPlanParser::ROOT:
      case SubstraitPlanParser::NULLVAL:
      case SubstraitPlanParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(346);
        literal_basic_type();
        break;
      }

      case SubstraitPlanParser::LIST: {
        enterOuterAlt(_localctx, 2);
        setState(347);
        match(SubstraitPlanParser::LIST);
        setState(349);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SubstraitPlanParser::QUESTIONMARK) {
          setState(348);
          match(SubstraitPlanParser::QUESTIONMARK);
        }
        setState(351);
        match(SubstraitPlanParser::LEFTANGLEBRACKET);
        setState(353);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 8022089323087968) != 0) || _la == SubstraitPlanParser::IDENTIFIER) {
          setState(352);
          literal_complex_type();
        }
        setState(355);
        match(SubstraitPlanParser::RIGHTANGLEBRACKET);
        break;
      }

      case SubstraitPlanParser::MAP: {
        enterOuterAlt(_localctx, 3);
        setState(356);
        match(SubstraitPlanParser::MAP);
        setState(358);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SubstraitPlanParser::QUESTIONMARK) {
          setState(357);
          match(SubstraitPlanParser::QUESTIONMARK);
        }
        setState(360);
        match(SubstraitPlanParser::LEFTANGLEBRACKET);
        setState(362);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
        case 1: {
          setState(361);
          literal_basic_type();
          break;
        }

        default:
          break;
        }
        setState(365);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SubstraitPlanParser::COMMA) {
          setState(364);
          match(SubstraitPlanParser::COMMA);
        }
        setState(368);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 8022089323087968) != 0) || _la == SubstraitPlanParser::IDENTIFIER) {
          setState(367);
          literal_complex_type();
        }
        setState(370);
        match(SubstraitPlanParser::RIGHTANGLEBRACKET);
        break;
      }

      case SubstraitPlanParser::STRUCT: {
        enterOuterAlt(_localctx, 4);
        setState(371);
        match(SubstraitPlanParser::STRUCT);
        setState(373);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SubstraitPlanParser::QUESTIONMARK) {
          setState(372);
          match(SubstraitPlanParser::QUESTIONMARK);
        }
        setState(375);
        match(SubstraitPlanParser::LEFTANGLEBRACKET);
        setState(377);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 8022089323087968) != 0) || _la == SubstraitPlanParser::IDENTIFIER) {
          setState(376);
          literal_complex_type();
        }
        setState(383);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SubstraitPlanParser::COMMA) {
          setState(379);
          match(SubstraitPlanParser::COMMA);
          setState(380);
          literal_complex_type();
          setState(385);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(386);
        match(SubstraitPlanParser::RIGHTANGLEBRACKET);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Literal_specifierContext ------------------------------------------------------------------

SubstraitPlanParser::Literal_specifierContext::Literal_specifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Literal_specifierContext::LEFTANGLEBRACKET() {
  return getToken(SubstraitPlanParser::LEFTANGLEBRACKET, 0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::Literal_specifierContext::NUMBER() {
  return getTokens(SubstraitPlanParser::NUMBER);
}

tree::TerminalNode* SubstraitPlanParser::Literal_specifierContext::NUMBER(size_t i) {
  return getToken(SubstraitPlanParser::NUMBER, i);
}

tree::TerminalNode* SubstraitPlanParser::Literal_specifierContext::RIGHTANGLEBRACKET() {
  return getToken(SubstraitPlanParser::RIGHTANGLEBRACKET, 0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::Literal_specifierContext::COMMA() {
  return getTokens(SubstraitPlanParser::COMMA);
}

tree::TerminalNode* SubstraitPlanParser::Literal_specifierContext::COMMA(size_t i) {
  return getToken(SubstraitPlanParser::COMMA, i);
}


size_t SubstraitPlanParser::Literal_specifierContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleLiteral_specifier;
}


std::any SubstraitPlanParser::Literal_specifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitLiteral_specifier(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Literal_specifierContext* SubstraitPlanParser::literal_specifier() {
  Literal_specifierContext *_localctx = _tracker.createInstance<Literal_specifierContext>(_ctx, getState());
  enterRule(_localctx, 32, SubstraitPlanParser::RuleLiteral_specifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(389);
    match(SubstraitPlanParser::LEFTANGLEBRACKET);
    setState(390);
    match(SubstraitPlanParser::NUMBER);
    setState(395);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SubstraitPlanParser::COMMA) {
      setState(391);
      match(SubstraitPlanParser::COMMA);
      setState(392);
      match(SubstraitPlanParser::NUMBER);
      setState(397);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(398);
    match(SubstraitPlanParser::RIGHTANGLEBRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Map_literalContext ------------------------------------------------------------------

SubstraitPlanParser::Map_literalContext::Map_literalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Map_literalContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

std::vector<SubstraitPlanParser::Map_literal_valueContext *> SubstraitPlanParser::Map_literalContext::map_literal_value() {
  return getRuleContexts<SubstraitPlanParser::Map_literal_valueContext>();
}

SubstraitPlanParser::Map_literal_valueContext* SubstraitPlanParser::Map_literalContext::map_literal_value(size_t i) {
  return getRuleContext<SubstraitPlanParser::Map_literal_valueContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Map_literalContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::Map_literalContext::COMMA() {
  return getTokens(SubstraitPlanParser::COMMA);
}

tree::TerminalNode* SubstraitPlanParser::Map_literalContext::COMMA(size_t i) {
  return getToken(SubstraitPlanParser::COMMA, i);
}


size_t SubstraitPlanParser::Map_literalContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleMap_literal;
}


std::any SubstraitPlanParser::Map_literalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitMap_literal(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Map_literalContext* SubstraitPlanParser::map_literal() {
  Map_literalContext *_localctx = _tracker.createInstance<Map_literalContext>(_ctx, getState());
  enterRule(_localctx, 34, SubstraitPlanParser::RuleMap_literal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(413);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(400);
      match(SubstraitPlanParser::LEFTBRACE);
      setState(401);
      map_literal_value();
      setState(406);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SubstraitPlanParser::COMMA) {
        setState(402);
        match(SubstraitPlanParser::COMMA);
        setState(403);
        map_literal_value();
        setState(408);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(409);
      match(SubstraitPlanParser::RIGHTBRACE);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(411);
      match(SubstraitPlanParser::LEFTBRACE);
      setState(412);
      match(SubstraitPlanParser::RIGHTBRACE);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Map_literal_valueContext ------------------------------------------------------------------

SubstraitPlanParser::Map_literal_valueContext::Map_literal_valueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SubstraitPlanParser::ConstantContext *> SubstraitPlanParser::Map_literal_valueContext::constant() {
  return getRuleContexts<SubstraitPlanParser::ConstantContext>();
}

SubstraitPlanParser::ConstantContext* SubstraitPlanParser::Map_literal_valueContext::constant(size_t i) {
  return getRuleContext<SubstraitPlanParser::ConstantContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Map_literal_valueContext::COLON() {
  return getToken(SubstraitPlanParser::COLON, 0);
}


size_t SubstraitPlanParser::Map_literal_valueContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleMap_literal_value;
}


std::any SubstraitPlanParser::Map_literal_valueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitMap_literal_value(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Map_literal_valueContext* SubstraitPlanParser::map_literal_value() {
  Map_literal_valueContext *_localctx = _tracker.createInstance<Map_literal_valueContext>(_ctx, getState());
  enterRule(_localctx, 36, SubstraitPlanParser::RuleMap_literal_value);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(415);
    constant();
    setState(416);
    match(SubstraitPlanParser::COLON);
    setState(417);
    constant();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Struct_literalContext ------------------------------------------------------------------

SubstraitPlanParser::Struct_literalContext::Struct_literalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Struct_literalContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

std::vector<SubstraitPlanParser::ConstantContext *> SubstraitPlanParser::Struct_literalContext::constant() {
  return getRuleContexts<SubstraitPlanParser::ConstantContext>();
}

SubstraitPlanParser::ConstantContext* SubstraitPlanParser::Struct_literalContext::constant(size_t i) {
  return getRuleContext<SubstraitPlanParser::ConstantContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Struct_literalContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::Struct_literalContext::COMMA() {
  return getTokens(SubstraitPlanParser::COMMA);
}

tree::TerminalNode* SubstraitPlanParser::Struct_literalContext::COMMA(size_t i) {
  return getToken(SubstraitPlanParser::COMMA, i);
}


size_t SubstraitPlanParser::Struct_literalContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleStruct_literal;
}


std::any SubstraitPlanParser::Struct_literalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitStruct_literal(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Struct_literalContext* SubstraitPlanParser::struct_literal() {
  Struct_literalContext *_localctx = _tracker.createInstance<Struct_literalContext>(_ctx, getState());
  enterRule(_localctx, 38, SubstraitPlanParser::RuleStruct_literal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(432);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 48, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(419);
      match(SubstraitPlanParser::LEFTBRACE);
      setState(420);
      constant();
      setState(425);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == SubstraitPlanParser::COMMA) {
        setState(421);
        match(SubstraitPlanParser::COMMA);
        setState(422);
        constant();
        setState(427);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(428);
      match(SubstraitPlanParser::RIGHTBRACE);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(430);
      match(SubstraitPlanParser::LEFTBRACE);
      setState(431);
      match(SubstraitPlanParser::RIGHTBRACE);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Column_nameContext ------------------------------------------------------------------

SubstraitPlanParser::Column_nameContext::Column_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SubstraitPlanParser::IdContext *> SubstraitPlanParser::Column_nameContext::id() {
  return getRuleContexts<SubstraitPlanParser::IdContext>();
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Column_nameContext::id(size_t i) {
  return getRuleContext<SubstraitPlanParser::IdContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Column_nameContext::PERIOD() {
  return getToken(SubstraitPlanParser::PERIOD, 0);
}


size_t SubstraitPlanParser::Column_nameContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleColumn_name;
}


std::any SubstraitPlanParser::Column_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitColumn_name(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Column_nameContext* SubstraitPlanParser::column_name() {
  Column_nameContext *_localctx = _tracker.createInstance<Column_nameContext>(_ctx, getState());
  enterRule(_localctx, 40, SubstraitPlanParser::RuleColumn_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(437);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx)) {
    case 1: {
      setState(434);
      id();
      setState(435);
      match(SubstraitPlanParser::PERIOD);
      break;
    }

    default:
      break;
    }
    setState(439);
    id();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Source_referenceContext ------------------------------------------------------------------

SubstraitPlanParser::Source_referenceContext::Source_referenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Source_referenceContext::SOURCE() {
  return getToken(SubstraitPlanParser::SOURCE, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Source_referenceContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}


size_t SubstraitPlanParser::Source_referenceContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleSource_reference;
}


std::any SubstraitPlanParser::Source_referenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitSource_reference(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Source_referenceContext* SubstraitPlanParser::source_reference() {
  Source_referenceContext *_localctx = _tracker.createInstance<Source_referenceContext>(_ctx, getState());
  enterRule(_localctx, 42, SubstraitPlanParser::RuleSource_reference);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(441);
    match(SubstraitPlanParser::SOURCE);
    setState(442);
    id();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- File_locationContext ------------------------------------------------------------------

SubstraitPlanParser::File_locationContext::File_locationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::File_locationContext::URI_FILE() {
  return getToken(SubstraitPlanParser::URI_FILE, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_locationContext::COLON() {
  return getToken(SubstraitPlanParser::COLON, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_locationContext::STRING() {
  return getToken(SubstraitPlanParser::STRING, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_locationContext::URI_PATH() {
  return getToken(SubstraitPlanParser::URI_PATH, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_locationContext::URI_PATH_GLOB() {
  return getToken(SubstraitPlanParser::URI_PATH_GLOB, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_locationContext::URI_FOLDER() {
  return getToken(SubstraitPlanParser::URI_FOLDER, 0);
}


size_t SubstraitPlanParser::File_locationContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleFile_location;
}


std::any SubstraitPlanParser::File_locationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitFile_location(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::File_locationContext* SubstraitPlanParser::file_location() {
  File_locationContext *_localctx = _tracker.createInstance<File_locationContext>(_ctx, getState());
  enterRule(_localctx, 44, SubstraitPlanParser::RuleFile_location);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(456);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SubstraitPlanParser::URI_FILE: {
        enterOuterAlt(_localctx, 1);
        setState(444);
        match(SubstraitPlanParser::URI_FILE);
        setState(445);
        match(SubstraitPlanParser::COLON);
        setState(446);
        match(SubstraitPlanParser::STRING);
        break;
      }

      case SubstraitPlanParser::URI_PATH: {
        enterOuterAlt(_localctx, 2);
        setState(447);
        match(SubstraitPlanParser::URI_PATH);
        setState(448);
        match(SubstraitPlanParser::COLON);
        setState(449);
        match(SubstraitPlanParser::STRING);
        break;
      }

      case SubstraitPlanParser::URI_PATH_GLOB: {
        enterOuterAlt(_localctx, 3);
        setState(450);
        match(SubstraitPlanParser::URI_PATH_GLOB);
        setState(451);
        match(SubstraitPlanParser::COLON);
        setState(452);
        match(SubstraitPlanParser::STRING);
        break;
      }

      case SubstraitPlanParser::URI_FOLDER: {
        enterOuterAlt(_localctx, 4);
        setState(453);
        match(SubstraitPlanParser::URI_FOLDER);
        setState(454);
        match(SubstraitPlanParser::COLON);
        setState(455);
        match(SubstraitPlanParser::STRING);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- File_detailContext ------------------------------------------------------------------

SubstraitPlanParser::File_detailContext::File_detailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::File_detailContext::PARTITION_INDEX() {
  return getToken(SubstraitPlanParser::PARTITION_INDEX, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_detailContext::COLON() {
  return getToken(SubstraitPlanParser::COLON, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_detailContext::NUMBER() {
  return getToken(SubstraitPlanParser::NUMBER, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_detailContext::START() {
  return getToken(SubstraitPlanParser::START, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_detailContext::LENGTH() {
  return getToken(SubstraitPlanParser::LENGTH, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_detailContext::ORC() {
  return getToken(SubstraitPlanParser::ORC, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_detailContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_detailContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::File_detailContext::PARQUET() {
  return getToken(SubstraitPlanParser::PARQUET, 0);
}

SubstraitPlanParser::File_locationContext* SubstraitPlanParser::File_detailContext::file_location() {
  return getRuleContext<SubstraitPlanParser::File_locationContext>(0);
}


size_t SubstraitPlanParser::File_detailContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleFile_detail;
}


std::any SubstraitPlanParser::File_detailContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitFile_detail(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::File_detailContext* SubstraitPlanParser::file_detail() {
  File_detailContext *_localctx = _tracker.createInstance<File_detailContext>(_ctx, getState());
  enterRule(_localctx, 46, SubstraitPlanParser::RuleFile_detail);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(476);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SubstraitPlanParser::PARTITION_INDEX: {
        enterOuterAlt(_localctx, 1);
        setState(458);
        match(SubstraitPlanParser::PARTITION_INDEX);
        setState(459);
        match(SubstraitPlanParser::COLON);
        setState(460);
        match(SubstraitPlanParser::NUMBER);
        break;
      }

      case SubstraitPlanParser::START: {
        enterOuterAlt(_localctx, 2);
        setState(461);
        match(SubstraitPlanParser::START);
        setState(462);
        match(SubstraitPlanParser::COLON);
        setState(463);
        match(SubstraitPlanParser::NUMBER);
        break;
      }

      case SubstraitPlanParser::LENGTH: {
        enterOuterAlt(_localctx, 3);
        setState(464);
        match(SubstraitPlanParser::LENGTH);
        setState(465);
        match(SubstraitPlanParser::COLON);
        setState(466);
        match(SubstraitPlanParser::NUMBER);
        break;
      }

      case SubstraitPlanParser::ORC: {
        enterOuterAlt(_localctx, 4);
        setState(467);
        match(SubstraitPlanParser::ORC);
        setState(468);
        match(SubstraitPlanParser::COLON);
        setState(469);
        match(SubstraitPlanParser::LEFTBRACE);
        setState(470);
        match(SubstraitPlanParser::RIGHTBRACE);
        break;
      }

      case SubstraitPlanParser::PARQUET: {
        enterOuterAlt(_localctx, 5);
        setState(471);
        match(SubstraitPlanParser::PARQUET);
        setState(472);
        match(SubstraitPlanParser::COLON);
        setState(473);
        match(SubstraitPlanParser::LEFTBRACE);
        setState(474);
        match(SubstraitPlanParser::RIGHTBRACE);
        break;
      }

      case SubstraitPlanParser::URI_FILE:
      case SubstraitPlanParser::URI_PATH:
      case SubstraitPlanParser::URI_PATH_GLOB:
      case SubstraitPlanParser::URI_FOLDER: {
        enterOuterAlt(_localctx, 6);
        setState(475);
        file_location();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FileContext ------------------------------------------------------------------

SubstraitPlanParser::FileContext::FileContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::FileContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::FileContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

std::vector<SubstraitPlanParser::File_detailContext *> SubstraitPlanParser::FileContext::file_detail() {
  return getRuleContexts<SubstraitPlanParser::File_detailContext>();
}

SubstraitPlanParser::File_detailContext* SubstraitPlanParser::FileContext::file_detail(size_t i) {
  return getRuleContext<SubstraitPlanParser::File_detailContext>(i);
}


size_t SubstraitPlanParser::FileContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleFile;
}


std::any SubstraitPlanParser::FileContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitFile(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::FileContext* SubstraitPlanParser::file() {
  FileContext *_localctx = _tracker.createInstance<FileContext>(_ctx, getState());
  enterRule(_localctx, 48, SubstraitPlanParser::RuleFile);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(478);
    match(SubstraitPlanParser::LEFTBRACE);
    setState(482);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 140462610448384) != 0)) {
      setState(479);
      file_detail();
      setState(484);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(485);
    match(SubstraitPlanParser::RIGHTBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Local_files_detailContext ------------------------------------------------------------------

SubstraitPlanParser::Local_files_detailContext::Local_files_detailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Local_files_detailContext::ADVANCED_EXTENSION() {
  return getToken(SubstraitPlanParser::ADVANCED_EXTENSION, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Local_files_detailContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::Local_files_detailContext::ITEMS() {
  return getToken(SubstraitPlanParser::ITEMS, 0);
}

tree::TerminalNode* SubstraitPlanParser::Local_files_detailContext::EQUAL() {
  return getToken(SubstraitPlanParser::EQUAL, 0);
}

tree::TerminalNode* SubstraitPlanParser::Local_files_detailContext::LEFTBRACKET() {
  return getToken(SubstraitPlanParser::LEFTBRACKET, 0);
}

std::vector<SubstraitPlanParser::FileContext *> SubstraitPlanParser::Local_files_detailContext::file() {
  return getRuleContexts<SubstraitPlanParser::FileContext>();
}

SubstraitPlanParser::FileContext* SubstraitPlanParser::Local_files_detailContext::file(size_t i) {
  return getRuleContext<SubstraitPlanParser::FileContext>(i);
}

tree::TerminalNode* SubstraitPlanParser::Local_files_detailContext::RIGHTBRACKET() {
  return getToken(SubstraitPlanParser::RIGHTBRACKET, 0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::Local_files_detailContext::COMMA() {
  return getTokens(SubstraitPlanParser::COMMA);
}

tree::TerminalNode* SubstraitPlanParser::Local_files_detailContext::COMMA(size_t i) {
  return getToken(SubstraitPlanParser::COMMA, i);
}


size_t SubstraitPlanParser::Local_files_detailContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleLocal_files_detail;
}


std::any SubstraitPlanParser::Local_files_detailContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitLocal_files_detail(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Local_files_detailContext* SubstraitPlanParser::local_files_detail() {
  Local_files_detailContext *_localctx = _tracker.createInstance<Local_files_detailContext>(_ctx, getState());
  enterRule(_localctx, 50, SubstraitPlanParser::RuleLocal_files_detail);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(505);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SubstraitPlanParser::ADVANCED_EXTENSION: {
        enterOuterAlt(_localctx, 1);
        setState(487);
        match(SubstraitPlanParser::ADVANCED_EXTENSION);
        setState(488);
        id();
        break;
      }

      case SubstraitPlanParser::ITEMS: {
        enterOuterAlt(_localctx, 2);
        setState(489);
        match(SubstraitPlanParser::ITEMS);
        setState(490);
        match(SubstraitPlanParser::EQUAL);
        setState(491);
        match(SubstraitPlanParser::LEFTBRACKET);
        setState(492);
        file();
        setState(497);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(493);
            match(SubstraitPlanParser::COMMA);
            setState(494);
            file(); 
          }
          setState(499);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx);
        }
        setState(501);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SubstraitPlanParser::COMMA) {
          setState(500);
          match(SubstraitPlanParser::COMMA);
        }
        setState(503);
        match(SubstraitPlanParser::RIGHTBRACKET);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Named_table_detailContext ------------------------------------------------------------------

SubstraitPlanParser::Named_table_detailContext::Named_table_detailContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Named_table_detailContext::ADVANCED_EXTENSION() {
  return getToken(SubstraitPlanParser::ADVANCED_EXTENSION, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Named_table_detailContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::Named_table_detailContext::NAMES() {
  return getToken(SubstraitPlanParser::NAMES, 0);
}

tree::TerminalNode* SubstraitPlanParser::Named_table_detailContext::EQUAL() {
  return getToken(SubstraitPlanParser::EQUAL, 0);
}

tree::TerminalNode* SubstraitPlanParser::Named_table_detailContext::LEFTBRACKET() {
  return getToken(SubstraitPlanParser::LEFTBRACKET, 0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::Named_table_detailContext::STRING() {
  return getTokens(SubstraitPlanParser::STRING);
}

tree::TerminalNode* SubstraitPlanParser::Named_table_detailContext::STRING(size_t i) {
  return getToken(SubstraitPlanParser::STRING, i);
}

tree::TerminalNode* SubstraitPlanParser::Named_table_detailContext::RIGHTBRACKET() {
  return getToken(SubstraitPlanParser::RIGHTBRACKET, 0);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::Named_table_detailContext::COMMA() {
  return getTokens(SubstraitPlanParser::COMMA);
}

tree::TerminalNode* SubstraitPlanParser::Named_table_detailContext::COMMA(size_t i) {
  return getToken(SubstraitPlanParser::COMMA, i);
}


size_t SubstraitPlanParser::Named_table_detailContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleNamed_table_detail;
}


std::any SubstraitPlanParser::Named_table_detailContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitNamed_table_detail(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Named_table_detailContext* SubstraitPlanParser::named_table_detail() {
  Named_table_detailContext *_localctx = _tracker.createInstance<Named_table_detailContext>(_ctx, getState());
  enterRule(_localctx, 52, SubstraitPlanParser::RuleNamed_table_detail);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(524);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SubstraitPlanParser::ADVANCED_EXTENSION: {
        enterOuterAlt(_localctx, 1);
        setState(507);
        match(SubstraitPlanParser::ADVANCED_EXTENSION);
        setState(508);
        id();
        break;
      }

      case SubstraitPlanParser::NAMES: {
        enterOuterAlt(_localctx, 2);
        setState(509);
        match(SubstraitPlanParser::NAMES);
        setState(510);
        match(SubstraitPlanParser::EQUAL);
        setState(511);
        match(SubstraitPlanParser::LEFTBRACKET);
        setState(512);
        match(SubstraitPlanParser::STRING);
        setState(517);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(513);
            match(SubstraitPlanParser::COMMA);
            setState(514);
            match(SubstraitPlanParser::STRING); 
          }
          setState(519);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx);
        }
        setState(521);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == SubstraitPlanParser::COMMA) {
          setState(520);
          match(SubstraitPlanParser::COMMA);
        }
        setState(523);
        match(SubstraitPlanParser::RIGHTBRACKET);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Schema_definitionContext ------------------------------------------------------------------

SubstraitPlanParser::Schema_definitionContext::Schema_definitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Schema_definitionContext::SCHEMA() {
  return getToken(SubstraitPlanParser::SCHEMA, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Schema_definitionContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::Schema_definitionContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::Schema_definitionContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

std::vector<SubstraitPlanParser::Schema_itemContext *> SubstraitPlanParser::Schema_definitionContext::schema_item() {
  return getRuleContexts<SubstraitPlanParser::Schema_itemContext>();
}

SubstraitPlanParser::Schema_itemContext* SubstraitPlanParser::Schema_definitionContext::schema_item(size_t i) {
  return getRuleContext<SubstraitPlanParser::Schema_itemContext>(i);
}


size_t SubstraitPlanParser::Schema_definitionContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleSchema_definition;
}


std::any SubstraitPlanParser::Schema_definitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitSchema_definition(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Schema_definitionContext* SubstraitPlanParser::schema_definition() {
  Schema_definitionContext *_localctx = _tracker.createInstance<Schema_definitionContext>(_ctx, getState());
  enterRule(_localctx, 54, SubstraitPlanParser::RuleSchema_definition);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(526);
    match(SubstraitPlanParser::SCHEMA);
    setState(527);
    id();
    setState(528);
    match(SubstraitPlanParser::LEFTBRACE);
    setState(532);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 140789975189600) != 0) || _la == SubstraitPlanParser::IDENTIFIER) {
      setState(529);
      schema_item();
      setState(534);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(535);
    match(SubstraitPlanParser::RIGHTBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Schema_itemContext ------------------------------------------------------------------

SubstraitPlanParser::Schema_itemContext::Schema_itemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SubstraitPlanParser::IdContext *> SubstraitPlanParser::Schema_itemContext::id() {
  return getRuleContexts<SubstraitPlanParser::IdContext>();
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Schema_itemContext::id(size_t i) {
  return getRuleContext<SubstraitPlanParser::IdContext>(i);
}

SubstraitPlanParser::Literal_complex_typeContext* SubstraitPlanParser::Schema_itemContext::literal_complex_type() {
  return getRuleContext<SubstraitPlanParser::Literal_complex_typeContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::Schema_itemContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

tree::TerminalNode* SubstraitPlanParser::Schema_itemContext::NAMED() {
  return getToken(SubstraitPlanParser::NAMED, 0);
}


size_t SubstraitPlanParser::Schema_itemContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleSchema_item;
}


std::any SubstraitPlanParser::Schema_itemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitSchema_item(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Schema_itemContext* SubstraitPlanParser::schema_item() {
  Schema_itemContext *_localctx = _tracker.createInstance<Schema_itemContext>(_ctx, getState());
  enterRule(_localctx, 56, SubstraitPlanParser::RuleSchema_item);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(537);
    id();
    setState(538);
    literal_complex_type();
    setState(541);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SubstraitPlanParser::NAMED) {
      setState(539);
      match(SubstraitPlanParser::NAMED);
      setState(540);
      id();
    }
    setState(543);
    match(SubstraitPlanParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Source_definitionContext ------------------------------------------------------------------

SubstraitPlanParser::Source_definitionContext::Source_definitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Source_definitionContext::SOURCE() {
  return getToken(SubstraitPlanParser::SOURCE, 0);
}

SubstraitPlanParser::Read_typeContext* SubstraitPlanParser::Source_definitionContext::read_type() {
  return getRuleContext<SubstraitPlanParser::Read_typeContext>(0);
}


size_t SubstraitPlanParser::Source_definitionContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleSource_definition;
}


std::any SubstraitPlanParser::Source_definitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitSource_definition(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Source_definitionContext* SubstraitPlanParser::source_definition() {
  Source_definitionContext *_localctx = _tracker.createInstance<Source_definitionContext>(_ctx, getState());
  enterRule(_localctx, 58, SubstraitPlanParser::RuleSource_definition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(545);
    match(SubstraitPlanParser::SOURCE);
    setState(546);
    read_type();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Read_typeContext ------------------------------------------------------------------

SubstraitPlanParser::Read_typeContext::Read_typeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SubstraitPlanParser::Read_typeContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleRead_type;
}

void SubstraitPlanParser::Read_typeContext::copyFrom(Read_typeContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- NamedTableContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::NamedTableContext::NAMED_TABLE() {
  return getToken(SubstraitPlanParser::NAMED_TABLE, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::NamedTableContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::NamedTableContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::NamedTableContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

std::vector<SubstraitPlanParser::Named_table_detailContext *> SubstraitPlanParser::NamedTableContext::named_table_detail() {
  return getRuleContexts<SubstraitPlanParser::Named_table_detailContext>();
}

SubstraitPlanParser::Named_table_detailContext* SubstraitPlanParser::NamedTableContext::named_table_detail(size_t i) {
  return getRuleContext<SubstraitPlanParser::Named_table_detailContext>(i);
}

SubstraitPlanParser::NamedTableContext::NamedTableContext(Read_typeContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::NamedTableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitNamedTable(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LocalFilesContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::LocalFilesContext::LOCAL_FILES() {
  return getToken(SubstraitPlanParser::LOCAL_FILES, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::LocalFilesContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::LocalFilesContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::LocalFilesContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

std::vector<SubstraitPlanParser::Local_files_detailContext *> SubstraitPlanParser::LocalFilesContext::local_files_detail() {
  return getRuleContexts<SubstraitPlanParser::Local_files_detailContext>();
}

SubstraitPlanParser::Local_files_detailContext* SubstraitPlanParser::LocalFilesContext::local_files_detail(size_t i) {
  return getRuleContext<SubstraitPlanParser::Local_files_detailContext>(i);
}

SubstraitPlanParser::LocalFilesContext::LocalFilesContext(Read_typeContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::LocalFilesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitLocalFiles(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VirtualTableContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::VirtualTableContext::VIRTUAL_TABLE() {
  return getToken(SubstraitPlanParser::VIRTUAL_TABLE, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::VirtualTableContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::VirtualTableContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::VirtualTableContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

SubstraitPlanParser::VirtualTableContext::VirtualTableContext(Read_typeContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::VirtualTableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitVirtualTable(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExtensionTableContext ------------------------------------------------------------------

tree::TerminalNode* SubstraitPlanParser::ExtensionTableContext::EXTENSION_TABLE() {
  return getToken(SubstraitPlanParser::EXTENSION_TABLE, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::ExtensionTableContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::ExtensionTableContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::ExtensionTableContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

SubstraitPlanParser::ExtensionTableContext::ExtensionTableContext(Read_typeContext *ctx) { copyFrom(ctx); }


std::any SubstraitPlanParser::ExtensionTableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExtensionTable(this);
  else
    return visitor->visitChildren(this);
}
SubstraitPlanParser::Read_typeContext* SubstraitPlanParser::read_type() {
  Read_typeContext *_localctx = _tracker.createInstance<Read_typeContext>(_ctx, getState());
  enterRule(_localctx, 60, SubstraitPlanParser::RuleRead_type);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(580);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SubstraitPlanParser::LOCAL_FILES: {
        _localctx = _tracker.createInstance<SubstraitPlanParser::LocalFilesContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(548);
        match(SubstraitPlanParser::LOCAL_FILES);
        setState(549);
        id();
        setState(550);
        match(SubstraitPlanParser::LEFTBRACE);
        setState(554);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SubstraitPlanParser::ADVANCED_EXTENSION

        || _la == SubstraitPlanParser::ITEMS) {
          setState(551);
          local_files_detail();
          setState(556);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(557);
        match(SubstraitPlanParser::RIGHTBRACE);
        break;
      }

      case SubstraitPlanParser::VIRTUAL_TABLE: {
        _localctx = _tracker.createInstance<SubstraitPlanParser::VirtualTableContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(559);
        match(SubstraitPlanParser::VIRTUAL_TABLE);
        setState(560);
        id();
        setState(561);
        match(SubstraitPlanParser::LEFTBRACE);
        setState(562);
        match(SubstraitPlanParser::RIGHTBRACE);
        break;
      }

      case SubstraitPlanParser::NAMED_TABLE: {
        _localctx = _tracker.createInstance<SubstraitPlanParser::NamedTableContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(564);
        match(SubstraitPlanParser::NAMED_TABLE);
        setState(565);
        id();
        setState(566);
        match(SubstraitPlanParser::LEFTBRACE);
        setState(570);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == SubstraitPlanParser::ADVANCED_EXTENSION

        || _la == SubstraitPlanParser::NAMES) {
          setState(567);
          named_table_detail();
          setState(572);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(573);
        match(SubstraitPlanParser::RIGHTBRACE);
        break;
      }

      case SubstraitPlanParser::EXTENSION_TABLE: {
        _localctx = _tracker.createInstance<SubstraitPlanParser::ExtensionTableContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(575);
        match(SubstraitPlanParser::EXTENSION_TABLE);
        setState(576);
        id();
        setState(577);
        match(SubstraitPlanParser::LEFTBRACE);
        setState(578);
        match(SubstraitPlanParser::RIGHTBRACE);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionspaceContext ------------------------------------------------------------------

SubstraitPlanParser::ExtensionspaceContext::ExtensionspaceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::ExtensionspaceContext::EXTENSION_SPACE() {
  return getToken(SubstraitPlanParser::EXTENSION_SPACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::ExtensionspaceContext::LEFTBRACE() {
  return getToken(SubstraitPlanParser::LEFTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::ExtensionspaceContext::RIGHTBRACE() {
  return getToken(SubstraitPlanParser::RIGHTBRACE, 0);
}

tree::TerminalNode* SubstraitPlanParser::ExtensionspaceContext::URI() {
  return getToken(SubstraitPlanParser::URI, 0);
}

std::vector<SubstraitPlanParser::FunctionContext *> SubstraitPlanParser::ExtensionspaceContext::function() {
  return getRuleContexts<SubstraitPlanParser::FunctionContext>();
}

SubstraitPlanParser::FunctionContext* SubstraitPlanParser::ExtensionspaceContext::function(size_t i) {
  return getRuleContext<SubstraitPlanParser::FunctionContext>(i);
}


size_t SubstraitPlanParser::ExtensionspaceContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleExtensionspace;
}


std::any SubstraitPlanParser::ExtensionspaceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitExtensionspace(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::ExtensionspaceContext* SubstraitPlanParser::extensionspace() {
  ExtensionspaceContext *_localctx = _tracker.createInstance<ExtensionspaceContext>(_ctx, getState());
  enterRule(_localctx, 62, SubstraitPlanParser::RuleExtensionspace);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(582);
    match(SubstraitPlanParser::EXTENSION_SPACE);
    setState(584);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SubstraitPlanParser::URI) {
      setState(583);
      match(SubstraitPlanParser::URI);
    }
    setState(586);
    match(SubstraitPlanParser::LEFTBRACE);
    setState(590);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SubstraitPlanParser::FUNCTION) {
      setState(587);
      function();
      setState(592);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(593);
    match(SubstraitPlanParser::RIGHTBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionContext ------------------------------------------------------------------

SubstraitPlanParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::FunctionContext::FUNCTION() {
  return getToken(SubstraitPlanParser::FUNCTION, 0);
}

SubstraitPlanParser::NameContext* SubstraitPlanParser::FunctionContext::name() {
  return getRuleContext<SubstraitPlanParser::NameContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::FunctionContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

tree::TerminalNode* SubstraitPlanParser::FunctionContext::AS() {
  return getToken(SubstraitPlanParser::AS, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::FunctionContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}


size_t SubstraitPlanParser::FunctionContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleFunction;
}


std::any SubstraitPlanParser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::FunctionContext* SubstraitPlanParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 64, SubstraitPlanParser::RuleFunction);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(595);
    match(SubstraitPlanParser::FUNCTION);
    setState(596);
    name();
    setState(599);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SubstraitPlanParser::AS) {
      setState(597);
      match(SubstraitPlanParser::AS);
      setState(598);
      id();
    }
    setState(601);
    match(SubstraitPlanParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Sort_fieldContext ------------------------------------------------------------------

SubstraitPlanParser::Sort_fieldContext::Sort_fieldContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Sort_fieldContext::SORT() {
  return getToken(SubstraitPlanParser::SORT, 0);
}

SubstraitPlanParser::ExpressionContext* SubstraitPlanParser::Sort_fieldContext::expression() {
  return getRuleContext<SubstraitPlanParser::ExpressionContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::Sort_fieldContext::SEMICOLON() {
  return getToken(SubstraitPlanParser::SEMICOLON, 0);
}

tree::TerminalNode* SubstraitPlanParser::Sort_fieldContext::BY() {
  return getToken(SubstraitPlanParser::BY, 0);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::Sort_fieldContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}


size_t SubstraitPlanParser::Sort_fieldContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleSort_field;
}


std::any SubstraitPlanParser::Sort_fieldContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitSort_field(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Sort_fieldContext* SubstraitPlanParser::sort_field() {
  Sort_fieldContext *_localctx = _tracker.createInstance<Sort_fieldContext>(_ctx, getState());
  enterRule(_localctx, 66, SubstraitPlanParser::RuleSort_field);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(603);
    match(SubstraitPlanParser::SORT);
    setState(604);
    expression(0);
    setState(607);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SubstraitPlanParser::BY) {
      setState(605);
      match(SubstraitPlanParser::BY);
      setState(606);
      id();
    }
    setState(609);
    match(SubstraitPlanParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NameContext ------------------------------------------------------------------

SubstraitPlanParser::NameContext::NameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::NameContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}

tree::TerminalNode* SubstraitPlanParser::NameContext::COLON() {
  return getToken(SubstraitPlanParser::COLON, 0);
}

SubstraitPlanParser::SignatureContext* SubstraitPlanParser::NameContext::signature() {
  return getRuleContext<SubstraitPlanParser::SignatureContext>(0);
}


size_t SubstraitPlanParser::NameContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleName;
}


std::any SubstraitPlanParser::NameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitName(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::NameContext* SubstraitPlanParser::name() {
  NameContext *_localctx = _tracker.createInstance<NameContext>(_ctx, getState());
  enterRule(_localctx, 68, SubstraitPlanParser::RuleName);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(611);
    id();
    setState(612);
    match(SubstraitPlanParser::COLON);
    setState(614);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 140789975189600) != 0) || _la == SubstraitPlanParser::IDENTIFIER) {
      setState(613);
      signature();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SignatureContext ------------------------------------------------------------------

SubstraitPlanParser::SignatureContext::SignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::SignatureContext::id() {
  return getRuleContext<SubstraitPlanParser::IdContext>(0);
}


size_t SubstraitPlanParser::SignatureContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleSignature;
}


std::any SubstraitPlanParser::SignatureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitSignature(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::SignatureContext* SubstraitPlanParser::signature() {
  SignatureContext *_localctx = _tracker.createInstance<SignatureContext>(_ctx, getState());
  enterRule(_localctx, 70, SubstraitPlanParser::RuleSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(616);
    id();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdContext ------------------------------------------------------------------

SubstraitPlanParser::IdContext::IdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SubstraitPlanParser::Simple_idContext *> SubstraitPlanParser::IdContext::simple_id() {
  return getRuleContexts<SubstraitPlanParser::Simple_idContext>();
}

SubstraitPlanParser::Simple_idContext* SubstraitPlanParser::IdContext::simple_id(size_t i) {
  return getRuleContext<SubstraitPlanParser::Simple_idContext>(i);
}

std::vector<tree::TerminalNode *> SubstraitPlanParser::IdContext::UNDERSCORE() {
  return getTokens(SubstraitPlanParser::UNDERSCORE);
}

tree::TerminalNode* SubstraitPlanParser::IdContext::UNDERSCORE(size_t i) {
  return getToken(SubstraitPlanParser::UNDERSCORE, i);
}


size_t SubstraitPlanParser::IdContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleId;
}


std::any SubstraitPlanParser::IdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitId(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::IdContext* SubstraitPlanParser::id() {
  IdContext *_localctx = _tracker.createInstance<IdContext>(_ctx, getState());
  enterRule(_localctx, 72, SubstraitPlanParser::RuleId);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(618);
    simple_id();
    setState(627);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 70, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(620); 
        _errHandler->sync(this);
        _la = _input->LA(1);
        do {
          setState(619);
          match(SubstraitPlanParser::UNDERSCORE);
          setState(622); 
          _errHandler->sync(this);
          _la = _input->LA(1);
        } while (_la == SubstraitPlanParser::UNDERSCORE);
        setState(624);
        simple_id(); 
      }
      setState(629);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 70, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Simple_idContext ------------------------------------------------------------------

SubstraitPlanParser::Simple_idContext::Simple_idContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::IDENTIFIER() {
  return getToken(SubstraitPlanParser::IDENTIFIER, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::FILTER() {
  return getToken(SubstraitPlanParser::FILTER, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::ROOT() {
  return getToken(SubstraitPlanParser::ROOT, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::SOURCE() {
  return getToken(SubstraitPlanParser::SOURCE, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::SCHEMA() {
  return getToken(SubstraitPlanParser::SCHEMA, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::NULLVAL() {
  return getToken(SubstraitPlanParser::NULLVAL, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::SORT() {
  return getToken(SubstraitPlanParser::SORT, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::MEASURE() {
  return getToken(SubstraitPlanParser::MEASURE, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::GROUPING() {
  return getToken(SubstraitPlanParser::GROUPING, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::COUNT() {
  return getToken(SubstraitPlanParser::COUNT, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::TYPE() {
  return getToken(SubstraitPlanParser::TYPE, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::EMIT() {
  return getToken(SubstraitPlanParser::EMIT, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::NAMED() {
  return getToken(SubstraitPlanParser::NAMED, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::ALL() {
  return getToken(SubstraitPlanParser::ALL, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::ANY() {
  return getToken(SubstraitPlanParser::ANY, 0);
}

tree::TerminalNode* SubstraitPlanParser::Simple_idContext::COMPARISON() {
  return getToken(SubstraitPlanParser::COMPARISON, 0);
}


size_t SubstraitPlanParser::Simple_idContext::getRuleIndex() const {
  return SubstraitPlanParser::RuleSimple_id;
}


std::any SubstraitPlanParser::Simple_idContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SubstraitPlanParserVisitor*>(visitor))
    return parserVisitor->visitSimple_id(this);
  else
    return visitor->visitChildren(this);
}

SubstraitPlanParser::Simple_idContext* SubstraitPlanParser::simple_id() {
  Simple_idContext *_localctx = _tracker.createInstance<Simple_idContext>(_ctx, getState());
  enterRule(_localctx, 74, SubstraitPlanParser::RuleSimple_id);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(630);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 140789975189600) != 0) || _la == SubstraitPlanParser::IDENTIFIER)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool SubstraitPlanParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 3: return pipelineSempred(antlrcpp::downCast<PipelineContext *>(context), predicateIndex);
    case 11: return expressionSempred(antlrcpp::downCast<ExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool SubstraitPlanParser::pipelineSempred(PipelineContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

bool SubstraitPlanParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 1: return precpred(_ctx, 5);
    case 2: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void SubstraitPlanParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  substraitplanparserParserInitialize();
#else
  ::antlr4::internal::call_once(substraitplanparserParserOnceFlag, substraitplanparserParserInitialize);
#endif
}
