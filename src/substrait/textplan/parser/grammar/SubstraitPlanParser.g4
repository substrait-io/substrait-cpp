// SPDX-License-Identifier: Apache-2.0

// $antlr-format alignTrailingComments on, columnLimit 130, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments off
// $antlr-format useTab off, allowShortRulesOnASingleLine off, allowShortBlocksOnASingleLine on, alignSemicolons ownLine

parser grammar SubstraitPlanParser;

options { tokenVocab = SubstraitPlanLexer; }

// Follows directly after the standard #includes in h + cpp files.
@ parser :: postinclude
{
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}


plan
   : plan_detail* EOF
   ;

plan_detail
   : pipelines
   | relation
   | schema_definition
   | source_definition
   | extensionspace
   ;

pipelines
   : PIPELINES LEFTBRACE (pipeline SEMICOLON)* RIGHTBRACE
   ;

pipeline
   : pipeline ARROW relation_ref
   | relation_ref
   ;

// TODO -- Make the token order involving ids consistent between relations and other top-level entities.
relation
   : relation_type RELATION relation_ref LEFTBRACE relation_detail* RIGHTBRACE
   ;

relation_type
   : id
   ;

relation_ref
   : id (LEFTPAREN SCHEMA id RIGHTPAREN)?
   ;

relation_filter_behavior
   : id
   | id MINUS id
   | id id
   ;

relation_detail
   : COMMON SEMICOLON                                           # relationCommon
   | BASE_SCHEMA id SEMICOLON                                   # relationUsesSchema
   | relation_filter_behavior? FILTER expression SEMICOLON      # relationFilter
   | EXPRESSION expression SEMICOLON (AS id)?                   # relationExpression
   | ADVANCED_EXTENSION SEMICOLON                               # relationAdvancedExtension
   | source_reference SEMICOLON                                 # relationSourceReference
   ;

expression
   : id LEFTPAREN expression (COMMA expression)? COMMA? RIGHTPAREN  # expressionFunctionUse
   | constant                                                       # expressionConstant
   | column_name                                                    # expressionColumn
   | expression AS literal_complex_type                             # expressionCast
   ;

constant
   : NUMBER (UNDERSCORE literal_basic_type)?
   | STRING (UNDERSCORE literal_basic_type)?
   | map_literal (UNDERSCORE literal_complex_type)?
   | struct_literal (UNDERSCORE literal_complex_type)?
   | NULLVAL (UNDERSCORE literal_complex_type)?
   | TRUEVAL (UNDERSCORE literal_basic_type)?
   | FALSEVAL (UNDERSCORE literal_basic_type)?
   ;

literal_basic_type
   : id literal_specifier? QUESTIONMARK?
   ;

literal_complex_type
   : literal_basic_type
   | LIST QUESTIONMARK? LEFTANGLEBRACKET literal_complex_type? RIGHTANGLEBRACKET
   | MAP QUESTIONMARK? LEFTANGLEBRACKET literal_basic_type? COMMA? literal_complex_type? RIGHTANGLEBRACKET
   | STRUCT QUESTIONMARK? LEFTANGLEBRACKET literal_complex_type? (COMMA literal_complex_type)* RIGHTANGLEBRACKET
   ;

literal_specifier   // Rename to literal_basic_type_specifier?
   : LEFTANGLEBRACKET NUMBER (COMMA NUMBER)* RIGHTANGLEBRACKET
   ;

map_literal
   : LEFTBRACE map_literal_value (COMMA map_literal_value)* RIGHTBRACE
   | LEFTBRACE RIGHTBRACE
   ;

map_literal_value
   : constant COLON constant
   ;

struct_literal
   : LEFTBRACE constant (COMMA constant)* RIGHTBRACE
   | LEFTBRACE RIGHTBRACE
   ;

column_name
   : id
   ;

source_reference
   : SOURCE id
   ;

file_location
   : URI_FILE COLON STRING
   | URI_PATH COLON STRING
   | URI_PATH_GLOB COLON STRING
   | URI_FOLDER COLON STRING
   ;

file_detail
   : PARTITION_INDEX COLON NUMBER
   | START COLON NUMBER
   | LENGTH COLON NUMBER
   | file_location
   ;

file
   : LEFTBRACE file_detail* RIGHTBRACE
   ;

local_files_detail
   : ADVANCED_EXTENSION id
   | ITEMS EQUAL LEFTBRACKET file (COMMA file)* COMMA? RIGHTBRACKET
   ;

named_table_detail
   : ADVANCED_EXTENSION id
   | NAMES EQUAL LEFTBRACKET STRING (COMMA STRING)* COMMA? RIGHTBRACKET
   ;

schema_definition
   : SCHEMA id LEFTBRACE schema_item* RIGHTBRACE
   ;

schema_item
   : id column_type SEMICOLON
   ;

column_type
   : column_attribute* id
   ;

column_attribute
   : NULLABLE
   ;

source_definition
   : SOURCE read_type
   ;

read_type
   : LOCAL_FILES id LEFTBRACE local_files_detail* RIGHTBRACE  # localFiles
   | VIRTUAL_TABLE id LEFTBRACE RIGHTBRACE                    # virtualTable
   | NAMED_TABLE id LEFTBRACE named_table_detail* RIGHTBRACE  # namedTable
   | EXTENSION_TABLE id LEFTBRACE RIGHTBRACE                  # extensionTable
   ;

extensionspace
   : EXTENSION_SPACE URI? LEFTBRACE function* RIGHTBRACE
   ;

function
   : FUNCTION name (AS id)? SEMICOLON
   ;

name
   : id COLON signature
   ;

signature
   : id
   ;

// List keywords here to make them not reserved.
id
   : IDENTIFIER (UNDERSCORE+ IDENTIFIER)*
   | FILTER
   | SCHEMA
   ;
