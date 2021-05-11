#ifndef FUX_PARSER_H
#define FUX_PARSER_H

#include "AST.h"
#include "lexer.h"

typedef struct PARSER_STRUCT {
    lexer_T* lexer;
    token_T* current_token;
    token_T* prev_token;
} parser_T;


parser_T* init_parser(lexer_T* lexer);

void parser_eat(parser_T* parser, int token_type);

AST_T* parser_parse(parser_T* parser);

AST_T* parser_parse_statement(parser_T* parser);

AST_T* parser_parse_statements(parser_T* parser);

AST_T* parser_parse_expr(parser_T* parser);

AST_T* parser_parse_factor(parser_T* parser);

AST_T* parser_parse_term(parser_T* parser);

AST_T* parser_parse_function_call(parser_T* parser);

AST_T* parser_parse_variable_defintion(parser_T* parser);

AST_T* parser_parse_variable(parser_T* parser);

AST_T* parser_parse_string(parser_T* parser);

AST_T* parser_parse_id(parser_T* parser);

#endif 