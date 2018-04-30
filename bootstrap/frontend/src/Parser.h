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
    std::unique_ptr<AstNode> parse_stmt();
    std::unique_ptr<AstBlock> parse_block();
    std::unique_ptr<AstNode> parse_symbol();
    std::unique_ptr<AstString> parse_string();
    std::unique_ptr<AstNumber> parse_number();
    std::unique_ptr<AstBoolean> parse_boolean();
    std::unique_ptr<AstDec> parse_dec();
    std::unique_ptr<AstFn> parse_fn();
    std::unique_ptr<AstLoop> parse_loop();
    std::unique_ptr<AstContinue> parse_continue();
    std::unique_ptr<AstBreak> parse_break();
    std::unique_ptr<AstGet> parse_get();
    std::unique_ptr<AstSet> parse_set();
    std::unique_ptr<AstImpl> parse_impl();
    std::unique_ptr<AstAttribute> parse_at();
    std::unique_ptr<AstOp> parse_op();
    std::unique_ptr<AstExpr> parse_expr();
    const Token &next_token();

    std::vector<Token> tokens;
    std::vector<Error> errors;
    size_t token_index = 0;
};

#endif /* PARSER_H */
