#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "substrait_common" for configuration "Debug"
set_property(
  TARGET substrait_common
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  substrait_common
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/libsubstrait_common.a")

list(APPEND _cmake_import_check_targets substrait_common)
list(APPEND _cmake_import_check_files_for_substrait_common
     "${_IMPORT_PREFIX}/lib/libsubstrait_common.a")

# Import target "substrait_io" for configuration "Debug"
set_property(
  TARGET substrait_io
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  substrait_io
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libsubstrait_io.a")

list(APPEND _cmake_import_check_targets substrait_io)
list(APPEND _cmake_import_check_files_for_substrait_io
     "${_IMPORT_PREFIX}/lib/libsubstrait_io.a")

# Import target "plantransformer" for configuration "Debug"
set_property(
  TARGET plantransformer
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  plantransformer PROPERTIES IMPORTED_LOCATION_DEBUG
                             "${_IMPORT_PREFIX}/bin/plantransformer")

list(APPEND _cmake_import_check_targets plantransformer)
list(APPEND _cmake_import_check_files_for_plantransformer
     "${_IMPORT_PREFIX}/bin/plantransformer")

# Import target "substrait_type" for configuration "Debug"
set_property(
  TARGET substrait_type
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  substrait_type
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/libsubstrait_type.a")

list(APPEND _cmake_import_check_targets substrait_type)
list(APPEND _cmake_import_check_files_for_substrait_type
     "${_IMPORT_PREFIX}/lib/libsubstrait_type.a")

# Import target "substrait_expression" for configuration "Debug"
set_property(
  TARGET substrait_expression
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  substrait_expression
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/libsubstrait_expression.a")

list(APPEND _cmake_import_check_targets substrait_expression)
list(APPEND _cmake_import_check_files_for_substrait_expression
     "${_IMPORT_PREFIX}/lib/libsubstrait_expression.a")

# Import target "substrait_function" for configuration "Debug"
set_property(
  TARGET substrait_function
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  substrait_function
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/libsubstrait_function.a")

list(APPEND _cmake_import_check_targets substrait_function)
list(APPEND _cmake_import_check_files_for_substrait_function
     "${_IMPORT_PREFIX}/lib/libsubstrait_function.a")

# Import target "substrait_proto" for configuration "Debug"
set_property(
  TARGET substrait_proto
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  substrait_proto
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/libsubstrait_proto.a")

list(APPEND _cmake_import_check_targets substrait_proto)
list(APPEND _cmake_import_check_files_for_substrait_proto
     "${_IMPORT_PREFIX}/lib/libsubstrait_proto.a")

# Import target "planconverter" for configuration "Debug"
set_property(
  TARGET planconverter
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  planconverter PROPERTIES IMPORTED_LOCATION_DEBUG
                           "${_IMPORT_PREFIX}/bin/planconverter")

list(APPEND _cmake_import_check_targets planconverter)
list(APPEND _cmake_import_check_files_for_planconverter
     "${_IMPORT_PREFIX}/bin/planconverter")

# Import target "substrait_textplan_converter" for configuration "Debug"
set_property(
  TARGET substrait_textplan_converter
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  substrait_textplan_converter
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/libsubstrait_textplan_converter.a")

list(APPEND _cmake_import_check_targets substrait_textplan_converter)
list(APPEND _cmake_import_check_files_for_substrait_textplan_converter
     "${_IMPORT_PREFIX}/lib/libsubstrait_textplan_converter.a")

# Import target "substrait_base_proto_visitor" for configuration "Debug"
set_property(
  TARGET substrait_base_proto_visitor
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  substrait_base_proto_visitor
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/libsubstrait_base_proto_visitor.a")

list(APPEND _cmake_import_check_targets substrait_base_proto_visitor)
list(APPEND _cmake_import_check_files_for_substrait_base_proto_visitor
     "${_IMPORT_PREFIX}/lib/libsubstrait_base_proto_visitor.a")

# Import target "substrait_textplan_normalizer" for configuration "Debug"
set_property(
  TARGET substrait_textplan_normalizer
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  substrait_textplan_normalizer
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/libsubstrait_textplan_normalizer.a")

list(APPEND _cmake_import_check_targets substrait_textplan_normalizer)
list(APPEND _cmake_import_check_files_for_substrait_textplan_normalizer
     "${_IMPORT_PREFIX}/lib/libsubstrait_textplan_normalizer.a")

# Import target "textplan_grammar" for configuration "Debug"
set_property(
  TARGET textplan_grammar
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  textplan_grammar
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/libtextplan_grammar.a")

list(APPEND _cmake_import_check_targets textplan_grammar)
list(APPEND _cmake_import_check_files_for_textplan_grammar
     "${_IMPORT_PREFIX}/lib/libtextplan_grammar.a")

# Import target "planparser" for configuration "Debug"
set_property(
  TARGET planparser
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(planparser PROPERTIES IMPORTED_LOCATION_DEBUG
                                            "${_IMPORT_PREFIX}/bin/planparser")

list(APPEND _cmake_import_check_targets planparser)
list(APPEND _cmake_import_check_files_for_planparser
     "${_IMPORT_PREFIX}/bin/planparser")

# Import target "substrait_textplan_loader" for configuration "Debug"
set_property(
  TARGET substrait_textplan_loader
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  substrait_textplan_loader
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/libsubstrait_textplan_loader.a")

list(APPEND _cmake_import_check_targets substrait_textplan_loader)
list(APPEND _cmake_import_check_files_for_substrait_textplan_loader
     "${_IMPORT_PREFIX}/lib/libsubstrait_textplan_loader.a")

# Import target "error_listener" for configuration "Debug"
set_property(
  TARGET error_listener
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  error_listener
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG
             "${_IMPORT_PREFIX}/lib/liberror_listener.a")

list(APPEND _cmake_import_check_targets error_listener)
list(APPEND _cmake_import_check_files_for_error_listener
     "${_IMPORT_PREFIX}/lib/liberror_listener.a")

# Import target "parse_result" for configuration "Debug"
set_property(
  TARGET parse_result
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  parse_result
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libparse_result.a")

list(APPEND _cmake_import_check_targets parse_result)
list(APPEND _cmake_import_check_files_for_parse_result
     "${_IMPORT_PREFIX}/lib/libparse_result.a")

# Import target "symbol_table" for configuration "Debug"
set_property(
  TARGET symbol_table
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  symbol_table
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libsymbol_table.a")

list(APPEND _cmake_import_check_targets symbol_table)
list(APPEND _cmake_import_check_files_for_symbol_table
     "${_IMPORT_PREFIX}/lib/libsymbol_table.a")

# Import target "planloader" for configuration "Debug"
set_property(
  TARGET planloader
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  planloader
  PROPERTIES IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libplanloader.so"
             IMPORTED_SONAME_DEBUG "libplanloader.so")

list(APPEND _cmake_import_check_targets planloader)
list(APPEND _cmake_import_check_files_for_planloader
     "${_IMPORT_PREFIX}/lib/libplanloader.so")

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
