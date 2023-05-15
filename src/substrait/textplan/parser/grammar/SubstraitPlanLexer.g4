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
SCHEMA: 'SCHEMA';
RELATION: 'RELATION';
NULLABLE: 'NULLABLE';
PIPELINES: 'PIPELINES';

COMMON: 'COMMON';
BASE_SCHEMA: 'BASE_SCHEMA';
FILTER: 'FILTER';
PROJECTION: 'PROJECTION';
EXPRESSION: 'EXPRESSION';
ADVANCED_EXTENSION: 'ADVANCED_EXTENSION';

VIRTUAL_TABLE: 'VIRTUAL_TABLE';
LOCAL_FILES: 'LOCAL_FILES';
NAMED_TABLE: 'NAMED_TABLE';
EXTENSION_TABLE: 'EXTENSION_TABLE';

SOURCE: 'SOURCE';
ITEMS: 'ITEMS';
NAMES: 'NAMES';
URI_FILE: 'URI_FILE';
URI_PATH: 'URI_PATH';
URI_PATH_GLOB: 'URI_PATH_GLOB';
URI_FOLDER: 'URI_FOLDER';
PARTITION_INDEX: 'PARTITION_INDEX';
START: 'START';
LENGTH: 'LENGTH';
NULL: 'NULL';

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

COLUMN_TYPE
    : 'fp64'
    | 'fp32'
    | 'int8'
    | 'int16'
    | 'int32'
    | 'int64'
    | 'uint8'
    | 'uint16'
    | 'uint32'
    | 'uint64'
    | 'UNKNOWN'
    ;

IDENTIFIER
    : [A-Z][A-Z_0-9]*
    ;

NUMBER
    : MINUS? [0-9]+ ( PERIOD [0-9]+ )?
    | MINUS? [0-9]+ ( PERIOD [0-9]+ )? 'E' ('+' | MINUS) [0-9]+
    ;

STRING : '"' (ESC | ~["\\])* '"' ;
fragment ESC : '\\' (["\\/bfnrt] | UNICODE | HEXESCAPE) ;
fragment HEXESCAPE : 'x' HEX HEX ;
fragment UNICODE : 'u' LEFTBRACE HEX HEX+ RIGHTBRACE ;
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
fragment FILENAME: [A-Z0-9-.]+;

URI
    : SCHEME ':' ( '//' HOSTNAME '/')? FILENAME
    | FILENAME
    ;

EXTENSIONS_LEFTBRACE: '{' -> mode(DEFAULT_MODE), type(LEFTBRACE);

EXTENSIONS_SPACES: [ \u000B\t\r\n] -> channel(HIDDEN), type(SPACES);