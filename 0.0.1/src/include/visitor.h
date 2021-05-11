#ifndef FUX_VISITOR_H
#define FUX_VISITOR_H

#include "AST.h"

typedef struct VISITOR_STRUCT {
    AST_T** variable_definitions;
    size_t variable_definitions_size;
} visitor_T;

visitor_T* init_visitor();

AST_T* visitor_visit(visitor_T* visitor, AST_T* node);

AST_T* visitor_visit_variable_definition(visitor_T* visitor, AST_T* node);

AST_T* visitor_visit_variable(visitor_T* visitor, AST_T* node);

AST_T* visitor_visit_function_call(visitor_T* visitor, AST_T* node);

AST_T* visitor_visit_sring(visitor_T* visitor, AST_T* node);

AST_T* visitor_visit_compound(visitor_T* visitor, AST_T* node);


#endif