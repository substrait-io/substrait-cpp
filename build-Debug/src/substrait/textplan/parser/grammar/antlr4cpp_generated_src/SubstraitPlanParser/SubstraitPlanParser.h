
// Generated from SubstraitPlanParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"


#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


namespace io::substrait::textplan {


class  SubstraitPlanParser : public antlr4::Parser {
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
    RulePlan = 0, RulePlan_detail = 1, RulePipelines = 2, RulePipeline = 3, 
    RuleRelation = 4, RuleRoot_relation = 5, RuleRelation_type = 6, RuleRelation_ref = 7, 
    RuleRelation_filter_behavior = 8, RuleMeasure_detail = 9, RuleRelation_detail = 10, 
    RuleExpression = 11, RuleExpression_list = 12, RuleConstant = 13, RuleLiteral_basic_type = 14, 
    RuleLiteral_complex_type = 15, RuleLiteral_specifier = 16, RuleMap_literal = 17, 
    RuleMap_literal_value = 18, RuleStruct_literal = 19, RuleColumn_name = 20, 
    RuleSource_reference = 21, RuleFile_location = 22, RuleFile_detail = 23, 
    RuleFile = 24, RuleLocal_files_detail = 25, RuleNamed_table_detail = 26, 
    RuleSchema_definition = 27, RuleSchema_item = 28, RuleSource_definition = 29, 
    RuleRead_type = 30, RuleExtensionspace = 31, RuleFunction = 32, RuleSort_field = 33, 
    RuleName = 34, RuleSignature = 35, RuleId = 36, RuleSimple_id = 37
  };

  explicit SubstraitPlanParser(antlr4::TokenStream *input);

  SubstraitPlanParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~SubstraitPlanParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class PlanContext;
  class Plan_detailContext;
  class PipelinesContext;
  class PipelineContext;
  class RelationContext;
  class Root_relationContext;
  class Relation_typeContext;
  class Relation_refContext;
  class Relation_filter_behaviorContext;
  class Measure_detailContext;
  class Relation_detailContext;
  class ExpressionContext;
  class Expression_listContext;
  class ConstantContext;
  class Literal_basic_typeContext;
  class Literal_complex_typeContext;
  class Literal_specifierContext;
  class Map_literalContext;
  class Map_literal_valueContext;
  class Struct_literalContext;
  class Column_nameContext;
  class Source_referenceContext;
  class File_locationContext;
  class File_detailContext;
  class FileContext;
  class Local_files_detailContext;
  class Named_table_detailContext;
  class Schema_definitionContext;
  class Schema_itemContext;
  class Source_definitionContext;
  class Read_typeContext;
  class ExtensionspaceContext;
  class FunctionContext;
  class Sort_fieldContext;
  class NameContext;
  class SignatureContext;
  class IdContext;
  class Simple_idContext; 

  class  PlanContext : public antlr4::ParserRuleContext {
  public:
    PlanContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<Plan_detailContext *> plan_detail();
    Plan_detailContext* plan_detail(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PlanContext* plan();

  class  Plan_detailContext : public antlr4::ParserRuleContext {
  public:
    Plan_detailContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PipelinesContext *pipelines();
    RelationContext *relation();
    Root_relationContext *root_relation();
    Schema_definitionContext *schema_definition();
    Source_definitionContext *source_definition();
    ExtensionspaceContext *extensionspace();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Plan_detailContext* plan_detail();

  class  PipelinesContext : public antlr4::ParserRuleContext {
  public:
    PipelinesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PIPELINES();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();
    std::vector<PipelineContext *> pipeline();
    PipelineContext* pipeline(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SEMICOLON();
    antlr4::tree::TerminalNode* SEMICOLON(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PipelinesContext* pipelines();

  class  PipelineContext : public antlr4::ParserRuleContext {
  public:
    PipelineContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Relation_refContext *relation_ref();
    PipelineContext *pipeline();
    antlr4::tree::TerminalNode *ARROW();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PipelineContext* pipeline();
  PipelineContext* pipeline(int precedence);
  class  RelationContext : public antlr4::ParserRuleContext {
  public:
    RelationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Relation_typeContext *relation_type();
    antlr4::tree::TerminalNode *RELATION();
    Relation_refContext *relation_ref();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();
    std::vector<Relation_detailContext *> relation_detail();
    Relation_detailContext* relation_detail(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RelationContext* relation();

  class  Root_relationContext : public antlr4::ParserRuleContext {
  public:
    Root_relationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ROOT();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *NAMES();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *LEFTBRACKET();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *RIGHTBRACKET();
    antlr4::tree::TerminalNode *RIGHTBRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Root_relationContext* root_relation();

  class  Relation_typeContext : public antlr4::ParserRuleContext {
  public:
    Relation_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Relation_typeContext* relation_type();

  class  Relation_refContext : public antlr4::ParserRuleContext {
  public:
    Relation_refContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *LEFTPAREN();
    antlr4::tree::TerminalNode *SCHEMA();
    antlr4::tree::TerminalNode *RIGHTPAREN();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Relation_refContext* relation_ref();

  class  Relation_filter_behaviorContext : public antlr4::ParserRuleContext {
  public:
    Relation_filter_behaviorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *MINUS();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Relation_filter_behaviorContext* relation_filter_behavior();

  class  Measure_detailContext : public antlr4::ParserRuleContext {
  public:
    Measure_detailContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *MEASURE();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMICOLON();
    antlr4::tree::TerminalNode *ARROW();
    Literal_complex_typeContext *literal_complex_type();
    antlr4::tree::TerminalNode *ATSIGN();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *NAMED();
    antlr4::tree::TerminalNode *FILTER();
    antlr4::tree::TerminalNode *INVOCATION();
    Sort_fieldContext *sort_field();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Measure_detailContext* measure_detail();

  class  Relation_detailContext : public antlr4::ParserRuleContext {
  public:
    Relation_detailContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Relation_detailContext() = default;
    void copyFrom(Relation_detailContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  RelationSourceReferenceContext : public Relation_detailContext {
  public:
    RelationSourceReferenceContext(Relation_detailContext *ctx);

    Source_referenceContext *source_reference();
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationEmitContext : public Relation_detailContext {
  public:
    RelationEmitContext(Relation_detailContext *ctx);

    antlr4::tree::TerminalNode *EMIT();
    Column_nameContext *column_name();
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationFilterContext : public Relation_detailContext {
  public:
    RelationFilterContext(Relation_detailContext *ctx);

    antlr4::tree::TerminalNode *FILTER();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMICOLON();
    Relation_filter_behaviorContext *relation_filter_behavior();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationMeasureContext : public Relation_detailContext {
  public:
    RelationMeasureContext(Relation_detailContext *ctx);

    antlr4::tree::TerminalNode *MEASURE();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();
    std::vector<Measure_detailContext *> measure_detail();
    Measure_detailContext* measure_detail(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationUsesSchemaContext : public Relation_detailContext {
  public:
    RelationUsesSchemaContext(Relation_detailContext *ctx);

    antlr4::tree::TerminalNode *BASE_SCHEMA();
    IdContext *id();
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationJoinTypeContext : public Relation_detailContext {
  public:
    RelationJoinTypeContext(Relation_detailContext *ctx);

    antlr4::tree::TerminalNode *TYPE();
    IdContext *id();
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationAdvancedExtensionContext : public Relation_detailContext {
  public:
    RelationAdvancedExtensionContext(Relation_detailContext *ctx);

    antlr4::tree::TerminalNode *ADVANCED_EXTENSION();
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationExpressionContext : public Relation_detailContext {
  public:
    RelationExpressionContext(Relation_detailContext *ctx);

    antlr4::tree::TerminalNode *EXPRESSION();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMICOLON();
    antlr4::tree::TerminalNode *NAMED();
    IdContext *id();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationCountContext : public Relation_detailContext {
  public:
    RelationCountContext(Relation_detailContext *ctx);

    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationCommonContext : public Relation_detailContext {
  public:
    RelationCommonContext(Relation_detailContext *ctx);

    antlr4::tree::TerminalNode *COMMON();
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationSortContext : public Relation_detailContext {
  public:
    RelationSortContext(Relation_detailContext *ctx);

    Sort_fieldContext *sort_field();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  RelationGroupingContext : public Relation_detailContext {
  public:
    RelationGroupingContext(Relation_detailContext *ctx);

    antlr4::tree::TerminalNode *GROUPING();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMICOLON();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Relation_detailContext* relation_detail();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExpressionContext() = default;
    void copyFrom(ExpressionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ExpressionScalarSubqueryContext : public ExpressionContext {
  public:
    ExpressionScalarSubqueryContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *SUBQUERY();
    Relation_refContext *relation_ref();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExpressionConstantContext : public ExpressionContext {
  public:
    ExpressionConstantContext(ExpressionContext *ctx);

    ConstantContext *constant();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExpressionFunctionUseContext : public ExpressionContext {
  public:
    ExpressionFunctionUseContext(ExpressionContext *ctx);

    IdContext *id();
    antlr4::tree::TerminalNode *LEFTPAREN();
    antlr4::tree::TerminalNode *RIGHTPAREN();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *ARROW();
    Literal_complex_typeContext *literal_complex_type();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExpressionColumnContext : public ExpressionContext {
  public:
    ExpressionColumnContext(ExpressionContext *ctx);

    Column_nameContext *column_name();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExpressionSetComparisonSubqueryContext : public ExpressionContext {
  public:
    ExpressionSetComparisonSubqueryContext(ExpressionContext *ctx);

    ExpressionContext *expression();
    antlr4::tree::TerminalNode *COMPARISON();
    antlr4::tree::TerminalNode *SUBQUERY();
    Relation_refContext *relation_ref();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *ANY();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExpressionInPredicateSubqueryContext : public ExpressionContext {
  public:
    ExpressionInPredicateSubqueryContext(ExpressionContext *ctx);

    Expression_listContext *expression_list();
    antlr4::tree::TerminalNode *IN();
    antlr4::tree::TerminalNode *SUBQUERY();
    Relation_refContext *relation_ref();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExpressionCastContext : public ExpressionContext {
  public:
    ExpressionCastContext(ExpressionContext *ctx);

    ExpressionContext *expression();
    antlr4::tree::TerminalNode *AS();
    Literal_complex_typeContext *literal_complex_type();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExpressionSetPredicateSubqueryContext : public ExpressionContext {
  public:
    ExpressionSetPredicateSubqueryContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *IN();
    antlr4::tree::TerminalNode *SUBQUERY();
    Relation_refContext *relation_ref();
    antlr4::tree::TerminalNode *UNIQUE();
    antlr4::tree::TerminalNode *EXISTS();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExpressionContext* expression();
  ExpressionContext* expression(int precedence);
  class  Expression_listContext : public antlr4::ParserRuleContext {
  public:
    Expression_listContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFTPAREN();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *RIGHTPAREN();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Expression_listContext* expression_list();

  class  ConstantContext : public antlr4::ParserRuleContext {
  public:
    ConstantContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *UNDERSCORE();
    Literal_basic_typeContext *literal_basic_type();
    antlr4::tree::TerminalNode *STRING();
    Map_literalContext *map_literal();
    Literal_complex_typeContext *literal_complex_type();
    Struct_literalContext *struct_literal();
    antlr4::tree::TerminalNode *NULLVAL();
    antlr4::tree::TerminalNode *TRUEVAL();
    antlr4::tree::TerminalNode *FALSEVAL();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstantContext* constant();

  class  Literal_basic_typeContext : public antlr4::ParserRuleContext {
  public:
    Literal_basic_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();
    antlr4::tree::TerminalNode *QUESTIONMARK();
    Literal_specifierContext *literal_specifier();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Literal_basic_typeContext* literal_basic_type();

  class  Literal_complex_typeContext : public antlr4::ParserRuleContext {
  public:
    Literal_complex_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Literal_basic_typeContext *literal_basic_type();
    antlr4::tree::TerminalNode *LIST();
    antlr4::tree::TerminalNode *LEFTANGLEBRACKET();
    antlr4::tree::TerminalNode *RIGHTANGLEBRACKET();
    antlr4::tree::TerminalNode *QUESTIONMARK();
    std::vector<Literal_complex_typeContext *> literal_complex_type();
    Literal_complex_typeContext* literal_complex_type(size_t i);
    antlr4::tree::TerminalNode *MAP();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *STRUCT();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Literal_complex_typeContext* literal_complex_type();

  class  Literal_specifierContext : public antlr4::ParserRuleContext {
  public:
    Literal_specifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFTANGLEBRACKET();
    std::vector<antlr4::tree::TerminalNode *> NUMBER();
    antlr4::tree::TerminalNode* NUMBER(size_t i);
    antlr4::tree::TerminalNode *RIGHTANGLEBRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Literal_specifierContext* literal_specifier();

  class  Map_literalContext : public antlr4::ParserRuleContext {
  public:
    Map_literalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFTBRACE();
    std::vector<Map_literal_valueContext *> map_literal_value();
    Map_literal_valueContext* map_literal_value(size_t i);
    antlr4::tree::TerminalNode *RIGHTBRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Map_literalContext* map_literal();

  class  Map_literal_valueContext : public antlr4::ParserRuleContext {
  public:
    Map_literal_valueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ConstantContext *> constant();
    ConstantContext* constant(size_t i);
    antlr4::tree::TerminalNode *COLON();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Map_literal_valueContext* map_literal_value();

  class  Struct_literalContext : public antlr4::ParserRuleContext {
  public:
    Struct_literalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFTBRACE();
    std::vector<ConstantContext *> constant();
    ConstantContext* constant(size_t i);
    antlr4::tree::TerminalNode *RIGHTBRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Struct_literalContext* struct_literal();

  class  Column_nameContext : public antlr4::ParserRuleContext {
  public:
    Column_nameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    antlr4::tree::TerminalNode *PERIOD();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Column_nameContext* column_name();

  class  Source_referenceContext : public antlr4::ParserRuleContext {
  public:
    Source_referenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SOURCE();
    IdContext *id();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Source_referenceContext* source_reference();

  class  File_locationContext : public antlr4::ParserRuleContext {
  public:
    File_locationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *URI_FILE();
    antlr4::tree::TerminalNode *COLON();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *URI_PATH();
    antlr4::tree::TerminalNode *URI_PATH_GLOB();
    antlr4::tree::TerminalNode *URI_FOLDER();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  File_locationContext* file_location();

  class  File_detailContext : public antlr4::ParserRuleContext {
  public:
    File_detailContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PARTITION_INDEX();
    antlr4::tree::TerminalNode *COLON();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *START();
    antlr4::tree::TerminalNode *LENGTH();
    antlr4::tree::TerminalNode *ORC();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();
    antlr4::tree::TerminalNode *PARQUET();
    File_locationContext *file_location();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  File_detailContext* file_detail();

  class  FileContext : public antlr4::ParserRuleContext {
  public:
    FileContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();
    std::vector<File_detailContext *> file_detail();
    File_detailContext* file_detail(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FileContext* file();

  class  Local_files_detailContext : public antlr4::ParserRuleContext {
  public:
    Local_files_detailContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ADVANCED_EXTENSION();
    IdContext *id();
    antlr4::tree::TerminalNode *ITEMS();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *LEFTBRACKET();
    std::vector<FileContext *> file();
    FileContext* file(size_t i);
    antlr4::tree::TerminalNode *RIGHTBRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Local_files_detailContext* local_files_detail();

  class  Named_table_detailContext : public antlr4::ParserRuleContext {
  public:
    Named_table_detailContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ADVANCED_EXTENSION();
    IdContext *id();
    antlr4::tree::TerminalNode *NAMES();
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *LEFTBRACKET();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *RIGHTBRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Named_table_detailContext* named_table_detail();

  class  Schema_definitionContext : public antlr4::ParserRuleContext {
  public:
    Schema_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SCHEMA();
    IdContext *id();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();
    std::vector<Schema_itemContext *> schema_item();
    Schema_itemContext* schema_item(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Schema_definitionContext* schema_definition();

  class  Schema_itemContext : public antlr4::ParserRuleContext {
  public:
    Schema_itemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    Literal_complex_typeContext *literal_complex_type();
    antlr4::tree::TerminalNode *SEMICOLON();
    antlr4::tree::TerminalNode *NAMED();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Schema_itemContext* schema_item();

  class  Source_definitionContext : public antlr4::ParserRuleContext {
  public:
    Source_definitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SOURCE();
    Read_typeContext *read_type();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Source_definitionContext* source_definition();

  class  Read_typeContext : public antlr4::ParserRuleContext {
  public:
    Read_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Read_typeContext() = default;
    void copyFrom(Read_typeContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  NamedTableContext : public Read_typeContext {
  public:
    NamedTableContext(Read_typeContext *ctx);

    antlr4::tree::TerminalNode *NAMED_TABLE();
    IdContext *id();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();
    std::vector<Named_table_detailContext *> named_table_detail();
    Named_table_detailContext* named_table_detail(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LocalFilesContext : public Read_typeContext {
  public:
    LocalFilesContext(Read_typeContext *ctx);

    antlr4::tree::TerminalNode *LOCAL_FILES();
    IdContext *id();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();
    std::vector<Local_files_detailContext *> local_files_detail();
    Local_files_detailContext* local_files_detail(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  VirtualTableContext : public Read_typeContext {
  public:
    VirtualTableContext(Read_typeContext *ctx);

    antlr4::tree::TerminalNode *VIRTUAL_TABLE();
    IdContext *id();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExtensionTableContext : public Read_typeContext {
  public:
    ExtensionTableContext(Read_typeContext *ctx);

    antlr4::tree::TerminalNode *EXTENSION_TABLE();
    IdContext *id();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Read_typeContext* read_type();

  class  ExtensionspaceContext : public antlr4::ParserRuleContext {
  public:
    ExtensionspaceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXTENSION_SPACE();
    antlr4::tree::TerminalNode *LEFTBRACE();
    antlr4::tree::TerminalNode *RIGHTBRACE();
    antlr4::tree::TerminalNode *URI();
    std::vector<FunctionContext *> function();
    FunctionContext* function(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionspaceContext* extensionspace();

  class  FunctionContext : public antlr4::ParserRuleContext {
  public:
    FunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FUNCTION();
    NameContext *name();
    antlr4::tree::TerminalNode *SEMICOLON();
    antlr4::tree::TerminalNode *AS();
    IdContext *id();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionContext* function();

  class  Sort_fieldContext : public antlr4::ParserRuleContext {
  public:
    Sort_fieldContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SORT();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *SEMICOLON();
    antlr4::tree::TerminalNode *BY();
    IdContext *id();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Sort_fieldContext* sort_field();

  class  NameContext : public antlr4::ParserRuleContext {
  public:
    NameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();
    antlr4::tree::TerminalNode *COLON();
    SignatureContext *signature();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NameContext* name();

  class  SignatureContext : public antlr4::ParserRuleContext {
  public:
    SignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignatureContext* signature();

  class  IdContext : public antlr4::ParserRuleContext {
  public:
    IdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Simple_idContext *> simple_id();
    Simple_idContext* simple_id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> UNDERSCORE();
    antlr4::tree::TerminalNode* UNDERSCORE(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdContext* id();

  class  Simple_idContext : public antlr4::ParserRuleContext {
  public:
    Simple_idContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *FILTER();
    antlr4::tree::TerminalNode *ROOT();
    antlr4::tree::TerminalNode *SOURCE();
    antlr4::tree::TerminalNode *SCHEMA();
    antlr4::tree::TerminalNode *NULLVAL();
    antlr4::tree::TerminalNode *SORT();
    antlr4::tree::TerminalNode *MEASURE();
    antlr4::tree::TerminalNode *GROUPING();
    antlr4::tree::TerminalNode *COUNT();
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *EMIT();
    antlr4::tree::TerminalNode *NAMED();
    antlr4::tree::TerminalNode *ALL();
    antlr4::tree::TerminalNode *ANY();
    antlr4::tree::TerminalNode *COMPARISON();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Simple_idContext* simple_id();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool pipelineSempred(PipelineContext *_localctx, size_t predicateIndex);
  bool expressionSempred(ExpressionContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

}  // namespace io::substrait::textplan
