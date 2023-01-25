// $antlr-format alignTrailingComments on, columnLimit 130, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments off
// $antlr-format useTab off, allowShortRulesOnASingleLine off, allowShortBlocksOnASingleLine on, alignSemicolons ownLine

parser grammar SubstraitParser;

options {
   tokenVocab = SubstraitLexer;
}

// MEGAHACK -- Flip so top level grammar items are on top.

plan: plan_detail* EOF
;

plan_detail : function
| extensionspace
| schema_definition
| relation
| pipelines
;

pipelines: PIPELINES LEFTBRACE (pipeline SEMICOLON)* RIGHTBRACE
;

pipeline: pipeline ARROW relation_ref
        | relation_ref
;

extensionspace: EXTENSION_SPACE URI? LEFTBRACE function* RIGHTBRACE
;

function: FUNCTION name uri? (AS id)? SEMICOLON
;

name: id COLON signature
;

signature: id
;

uri: URI
;

schema_definition: SCHEMA id LEFTBRACE schema_item* RIGHTBRACE
;

column_attribute: NULL
;

column_type: column_attribute* COLUMN_TYPE
;

schema_item: id column_type SEMICOLON
;

relation: RELATION relation_ref LEFTBRACE relation_detail* RIGHTBRACE
;

constant: NUMBER |
          STRING
;

column_name: id
;

operation: id LEFTPAREN operation (COMMA operation)? COMMA? RIGHTPAREN |
           constant |
           column_name
;

relation_detail: COMMON SEMICOLON
    | BASE_SCHEMA id SEMICOLON
    | FILTER operation SEMICOLON
    | PROJECTION SEMICOLON
    | ADVANCED_EXTENSION SEMICOLON
    | read_type
;

file_location: URI_FILE COLON STRING
               | URI_PATH COLON STRING
               | URI_PATH_GLOB COLON STRING
               | URI_FOLDER COLON STRING
;

file_detail: PARTITION_INDEX COLON NUMBER
| START COLON NUMBER
| LENGTH COLON NUMBER
| file_location
;

file: LEFTBRACE file_detail* RIGHTBRACE
;

local_files_detail: ADVANCED_EXTENSION id  // MEGAHACK -- Need to figure out how to include extension content.
    | ITEMS EQUAL LEFTBRACKET file (COMMA file)* COMMA? RIGHTBRACKET
;

local_files: LOCAL_FILES LEFTBRACE local_files_detail* RIGHTBRACE
;

read_type: local_files
| VIRTUAL_TABLE LEFTBRACE RIGHTBRACE
| NAMED_TABLE LEFTBRACE RIGHTBRACE
| EXTENSION_TABLE LEFTBRACE RIGHTBRACE
;

relation_ref: id (LEFTPAREN SCHEMA id RIGHTPAREN)?
;

// List keywords here to make them not reserved.
id: IDENTIFIER |
    FILTER |
    COLUMN_TYPE  // Temporary addition until grammar stops using id everywhere.
;
