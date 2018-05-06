#ifndef PARSER_H
#define PARSER_H

#include <cstddef>
#include <vector>
#include "Ast.h"
#include "Token.h"
#include "Error.h"

class Parser {
public:
    Ast parse(const std::vector<Token> &tokens);
    std::vector<Error> errors;

private:
    AstNode      *parse_stmt();
    AstBlock     *parse_block();
    AstNode      *parse_symbol();
    AstString    *parse_string();
    AstNumber    *parse_number();
    AstBoolean   *parse_boolean();
    AstArray     *parse_array();
    AstType      *parse_type();
    AstDec       *parse_dec();
    AstFn        *parse_fn(bool require_body = true);
    AstLoop      *parse_loop();
    AstIf        *parse_if();
    AstContinue  *parse_continue();
    AstBreak     *parse_break();
    AstStruct    *parse_struct();
    AstImpl      *parse_impl();
    AstAttribute *parse_at();
    AstAffix     *parse_affix();
    AstReturn    *parse_return();
    AstExtern    *parse_extern();
    AstNode      *parse_expr();
    AstNode      *parse_expr_primary();
    bool parse_params(std::vector<AstDec*> &result);
    bool parse_args(std::vector<AstNode*> &result);
    bool next_token();
    bool prev_token();
    bool accept(TokenType type);
    bool expect(TokenType type);

    std::vector<Token> tokens;
    int token_index = 0;
};

#endif /* PARSER_H */
