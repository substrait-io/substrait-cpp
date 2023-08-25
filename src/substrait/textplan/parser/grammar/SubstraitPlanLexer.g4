// SPDX-License-Identifier: Apache-2.0

// $antlr-format alignTrailingComments on, columnLimit 150, maxEmptyLinesToKeep 1, reflowComments off, useTab off
// $antlr-format allowShortRulesOnASingleLine on, alignSemicolons ownLine

lexer grammar SubstraitPlanLexer;

options {
    caseInsensitive = true;
}

@lexer::header {
// SPDX-License-Identifier: Apache-2.0
}

@lexer::postinclude {
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}

channels { CommentsChannel, DirectiveChannel }

tokens { SPACES }

EXTENSION_SPACE: 'EXTENSION_SPACE' -> mode(EXTENSIONS);
FUNCTION: 'FUNCTION';
AS: 'AS';
NAMED: 'NAMED';
SCHEMA: 'SCHEMA';
RELATION: 'RELATION';
PIPELINES: 'PIPELINES';

COMMON: 'COMMON';
BASE_SCHEMA: 'BASE_SCHEMA';
FILTER: 'FILTER';
PROJECTION: 'PROJECTION';
EXPRESSION: 'EXPRESSION';
ADVANCED_EXTENSION: 'ADVANCED_EXTENSION';
GROUPING: 'GROUPING';
MEASURE: 'MEASURE';
INVOCATION: 'INVOCATION';
SORT: 'SORT';
BY: 'BY';
COUNT: 'COUNT';
TYPE: 'TYPE';
EMIT: 'EMIT';

SUBQUERY: 'SUBQUERY';
EXISTS: 'EXISTS';
UNIQUE: 'UNIQUE';
IN: 'IN';
OR: 'OR';
ALL: 'ALL';
COMPARISON: 'EQ'|'NE'|'LT'|'GT'|'LE'|'GE';

VIRTUAL_TABLE: 'VIRTUAL_TABLE';
LOCAL_FILES: 'LOCAL_FILES';
NAMED_TABLE: 'NAMED_TABLE';
EXTENSION_TABLE: 'EXTENSION_TABLE';

SOURCE: 'SOURCE';
ROOT: 'ROOT';
ITEMS: 'ITEMS';
NAMES: 'NAMES';
URI_FILE: 'URI_FILE';
URI_PATH: 'URI_PATH';
URI_PATH_GLOB: 'URI_PATH_GLOB';
URI_FOLDER: 'URI_FOLDER';
PARTITION_INDEX: 'PARTITION_INDEX';
START: 'START';
LENGTH: 'LENGTH';
ORC: 'ORC';
NULLVAL: 'NULL';
TRUEVAL: 'TRUE';
FALSEVAL: 'FALSE';

LIST: 'LIST';
MAP: 'MAP';
STRUCT: 'STRUCT';

ARROW: '->';
COLON: ':';
SEMICOLON: ';';
LEFTBRACE: '{';
RIGHTBRACE: '}';
LEFTPAREN: '(';
RIGHTPAREN: ')';
fragment QUOTE: '"';
COMMA: ',';
PERIOD: '.';
EQUAL: '=';
LEFTBRACKET: '[';
RIGHTBRACKET: ']';
UNDERSCORE: '_';
MINUS: '-';
LEFTANGLEBRACKET: '<';
RIGHTANGLEBRACKET: '>';
QUESTIONMARK: '?';
ATSIGN: '@';

IDENTIFIER
    : [A-Z][A-Z0-9$]*
    ;

NUMBER
    : MINUS? [0-9]+ ( PERIOD [0-9]+ )?
    | MINUS? [0-9]+ ( PERIOD [0-9]+ )? 'E' ('+' | MINUS) [0-9]+
    ;

STRING : '"' (ESCAPEDQUOTE | ~["])* '"' ;
fragment ESCAPEDQUOTE : '\\' '"' ;
fragment HEX : [0-9A-F] ;
fragment DIGIT : [0-9] ;

RAW_LITERAL_SINGLE_BACKTICK : '`' ~[`]+? '`' -> type(STRING) ;
RAW_LITERAL_DOUBLE_BACKTICK : '``' .+? '``' -> type(STRING) ;
RAW_LITERAL_TRIPLE_BACKTICK : '```' .+? '```' -> type(STRING) ;

SINGLE_LINE_COMMENT: '//' ~[\r\n]* (('\r'? '\n') | EOF) -> channel(HIDDEN);

SPACES: [ \u000B\t\r\n] -> channel(HIDDEN);

mode EXTENSIONS;
fragment SCHEME: [A-Z]+ ;
fragment HOSTNAME: [A-Z0-9-.]+ ;
fragment FILENAME: [A-Z0-9-._]+;
fragment PATH: FILENAME ( '/' FILENAME )*;

URI
    : SCHEME ':' ( '//' HOSTNAME '/' )? PATH
    | '/'? PATH
    ;

EXTENSIONS_LEFTBRACE: '{' -> mode(DEFAULT_MODE), type(LEFTBRACE);

EXTENSIONS_SPACES: [ \u000B\t\r\n] -> channel(HIDDEN), type(SPACES);
