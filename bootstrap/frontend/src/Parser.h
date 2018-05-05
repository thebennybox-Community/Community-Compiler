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

private:
    std::unique_ptr<AstNode>      parse_stmt();
    std::unique_ptr<AstBlock>     parse_block();
    std::unique_ptr<AstNode>      parse_symbol();
    std::unique_ptr<AstString>    parse_string();
    std::unique_ptr<AstNumber>    parse_number();
    std::unique_ptr<AstBoolean>   parse_boolean();
    std::unique_ptr<AstArray>     parse_array();
    std::unique_ptr<AstType>      parse_type();
    std::unique_ptr<AstDec>       parse_dec();
    std::unique_ptr<AstFn>        parse_fn(bool require_body = true);
    std::unique_ptr<AstLoop>      parse_loop();
    std::unique_ptr<AstIf>        parse_if();
    std::unique_ptr<AstContinue>  parse_continue();
    std::unique_ptr<AstBreak>     parse_break();
    std::unique_ptr<AstStruct>    parse_struct();
    std::unique_ptr<AstImpl>      parse_impl();
    std::unique_ptr<AstAttribute> parse_at();
    std::unique_ptr<AstAffix>     parse_affix();
    std::unique_ptr<AstReturn>    parse_return();
    std::unique_ptr<AstExtern>    parse_extern();
    std::unique_ptr<AstNode>      parse_expr();
    std::unique_ptr<AstNode>      parse_expr_primary();
    bool parse_params(std::vector<std::unique_ptr<AstDec>> &result);
    bool parse_args(std::vector<std::unique_ptr<AstNode>> &result);
    bool next_token();
    bool prev_token();
    bool accept(TokenType type);
    bool expect(TokenType type);

    std::vector<Token> tokens;
    std::vector<Error> errors;
    int token_index = 0;
};

#endif /* PARSER_H */
