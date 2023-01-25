// $antlr-format alignTrailingComments on, columnLimit 150, maxEmptyLinesToKeep 1, reflowComments off, useTab off
// $antlr-format allowShortRulesOnASingleLine on, alignSemicolons ownLine

lexer grammar SubstraitLexer;

options {
    caseInsensitive = true;
}

EXTENSION_SPACE: 'EXTENSION_SPACE';
FUNCTION: 'FUNCTION';
AS: 'AS';
SCHEMA: 'SCHEMA';
RELATION: 'RELATION';
NULL: 'NULL';
PIPELINES: 'PIPELINES';

COMMON: 'COMMON';
BASE_SCHEMA: 'BASE_SCHEMA';
FILTER: 'FILTER';
PROJECTION: 'PROJECTION';
ADVANCED_EXTENSION: 'ADVANCED_EXTENSION';

VIRTUAL_TABLE: 'VIRTUAL_TABLE';
LOCAL_FILES: 'LOCAL_FILES';
NAMED_TABLE: 'NAMED_TABLE';
EXTENSION_TABLE: 'EXTENSION_TABLE';

ITEMS: 'ITEMS';
URI_FILE: 'URI_FILE';
URI_PATH: 'URI_PATH';
URI_PATH_GLOB: 'URI_PATH_GLOB';
URI_FOLDER: 'URI_FOLDER';
PARTITION_INDEX: 'PARTITION_INDEX';
START: 'START';
LENGTH: 'LENGTH';

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

COLUMN_TYPE: 'fp64'
|             'fp32'
|             'int8'
|             'int16'
|             'int32'
|             'int64'
|             'uint8'
|             'uint16'
|             'uint32'
|             'uint64'
;

IDENTIFIER:
    [A-Z][A-Z_0-9]*
;

URI:
    'http://sample'
    | 'boolean.yaml'
;

NUMBER: [0-9]+ ( PERIOD [0-9]+ )?
;

STRING : '"' (ESC | ~["\\])* '"' ;
fragment ESC : '\\' (["\\/bfnrt] | UNICODE) ;
fragment UNICODE : 'u' HEX HEX HEX HEX ;
fragment HEX : [0-9A-F] ;

SINGLE_LINE_COMMENT: '//' ~[\r\n]* (('\r'? '\n') | EOF) -> channel(HIDDEN);

SPACES: [ \u000B\t\r\n] -> channel(HIDDEN);
