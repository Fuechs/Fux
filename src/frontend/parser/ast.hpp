/**
 * @file ast.hpp
 * @author fuechs
 * @brief fux abstract syntax tree header
 * @version 0.1
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#pragma once

#include <list>
#include "../lexer/token.hpp"

enum AstType {
    ast_class_decl,
    ast_mutate_decl,
    ast_import_decl,
    ast_package_decl,
    ast_method_decl,
    ast_construct_decl,
    ast_label_decl,
    ast_operator_decl,
    ast_variable_decl,
    ast_value,
    ast_value_list,
    ast_utype_arg_list,
    ast_utype_arg_list_opt,
    ast_vector_array,
    ast_utype_arg,
    ast_utype_arg_opt,
    ast_expression,
    ast_array_expression,
    ast_primary_expr,
    ast_dotnotation_call_expr,
    ast_utype,
    ast_block,
    ast_finally_block,
    ast_assembly_block,
    ast_catch_clause,
    ast_method_return_type,
    ast_return_stmnt,
    ast_statement,
    ast_if_statement,
    ast_elseif_statement,
    ast_else_statement,
    ast_trycatch_statement,
    ast_throw_statement,
    ast_continue_statement,
    ast_break_statement,
    ast_goto_statement,
    ast_while_statement,
    ast_do_while_statement,
    ast_assembly_statement,
    ast_for_statement,
    ast_for_expresion_cond,
    ast_for_expresion_iter,
    ast_foreach_statement,
    ast_type_identifier,
    ast_refrence_pointer,
    ast_packagename,
    ast_literal,

    // encapsulated ASTs to make processing expressions easier
    ast_literal_e,
    ast_utype_class_e,
    ast_dot_not_e,
    ast_self_e,
    ast_base_e,
    ast_null_e,
    ast_new_e,
    ast_not_e,
    ast_post_inc_e,
    ast_arry_e,
    ast_dot_fn_e,
    ast_cast_e,
    ast_pre_inc_e,
    ast_paren_e,
    ast_vect_e,
    ast_add_e,
    ast_mult_e,
    ast_shift_e,
    ast_less_e,
    ast_equal_e,
    ast_and_e,
    ast_ques_e,
    ast_assign_e,
    ast_sizeof_e,

    ast_none
};

class AST {
public:
    AST(AST *parent, AstType type, int line, int col)
    : type(type), parent(parent), line(line), col(col), 
    numEntities(0), numASTs(0) 
    {  
        subASTs.init();
        entities.init();
    }

    AST()
    : type(ast_none), parent(NULL), line(0), col(0), 
    numEntities(0), numASTs(0)
    {
        subASTs.init();
        entities.init();
    }

    void encapsulate(AstType at);
    AstType getType();
    AST *getParent();
    AST *getSubAST(long at);
    AST *getLastSubAST();
    AST *getSubAST(AstType at);
    AST *getNextSubAST(AstType at);
    bool hasSubAST(AstType at);
    bool hasEntity(TokenType t);
    void freeSubASTS();
    long getEntityCount();
    Token getEntity(long at);
    Token getEntity(TokenType t);

    void addEntity(Token entity);
    void addAST(AST _ast);
    void free();

    void freeEntities();
    void freeLastSub();
    void freeLastEntity();
    void setASTType(AstType type);

    int line, col;
    long numEntities, numASTs;

private:
    AstType type;
    AST *parent;
    List<AST> subASTs;
    List<Token> entities;
};