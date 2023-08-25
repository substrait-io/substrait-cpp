# Parser Phases

There are four phases that the parser goes through to parse a textplan into its
internal representation (stored inside a ```SymbolTable`````).

* ```SubstraitPlanVisitor```
   * Finds the relations, processes functions, data files, and schemas. 
* ```SubstraitPlanPipelineVisitor```
   * Stores the connections between the relations as determined by the pipelines 
     section.
* ```SubstraitPlanRelationVisitor```
   * 
* ```SubstraitPlanSubqueryRelationVisitor```
   * Determines the input fields for subqueries. 

```SubstraitPlanTypeVisitor``` is a common visitor used by all but
```SubstraitPlanPipelineVisitor``` to do type lookups.

The phase order is implemented in ```ParserText.cpp```.
