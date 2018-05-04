#include "Parser.h"

Ast Parser::parse(const std::vector<Token> &tokens) {
    this->tokens = tokens;
    Ast ast;
    ast.root = std::make_unique<AstBlock>();

    while(this->token_index < this->tokens.size()) {
        std::unique_ptr<AstNode> statement = parse_stmt();
        if(this->errors.size() != 0) {
            ast.root->statements.push_back(std::move(statement));
        }
    }

    return ast;
}

std::unique_ptr<AstNode> Parser::parse_stmt() {
    const Token &tok = next_token();

    switch(tok.tokenType) {
        #define _(type, name) case TokenType::type: return parse_##name()

        _(OpenCurlyBracket, block);
        _(Symbol, symbol);
        _(StringLiteral, string);
        _(NumberLiteral, number);
        _(Boolean, boolean);
        _(Dec, dec);
        _(Fn, fn);
        _(Loop, loop);
        _(Continue, continue);
        _(Break, break);
        _(Get, get);
        _(Set, set);
        _(Impl, impl);
        _(At, at);
        _(Op, op);

        case TokenType::SemiColon: break; // TODO

        default:
            this->errors.push_back({ErrorType::UnexpectedToken, tok});
            break;

        #undef _
    }

    return nullptr;
}

std::unique_ptr<AstBlock> parse_block() {
    return nullptr; // TODO
}

std::unique_ptr<AstNode> parse_symbol() {
    return nullptr; // TODO
}

std::unique_ptr<AstString> parse_string() {
    return nullptr; // TODO
}

std::unique_ptr<AstNumber> parse_number() {
    return nullptr; // TODO
}

std::unique_ptr<AstBoolean> parse_boolean() {
    return nullptr; // TODO
}

std::unique_ptr<AstDec> parse_dec() {
    return nullptr; // TODO
}

std::unique_ptr<AstFn> parse_fn() {
    return nullptr; // TODO
}

std::unique_ptr<AstLoop> parse_loop() {
    return nullptr; // TODO
}

std::unique_ptr<AstContinue> parse_continue() {
    return nullptr; // TODO
}

std::unique_ptr<AstBreak> parse_break() {
    return nullptr; // TODO
}

std::unique_ptr<AstGet> parse_get() {
    return nullptr; // TODO
}

std::unique_ptr<AstSet> parse_set() {
    return nullptr; // TODO
}

std::unique_ptr<AstImpl> parse_impl() {
    return nullptr; // TODO
}

std::unique_ptr<AstAttribute> parse_at() {
    return nullptr; // TODO
}

std::unique_ptr<AstOp> parse_op() {
    return nullptr; // TODO
}

std::unique_ptr<AstExpr> parse_expr() {
    return nullptr; // TODO
}

const Token &Parser::next_token() {
    while(this->tokens[this->token_index].tokenType == TokenType::SingleLineComment ||
          this->tokens[this->token_index].tokenType == TokenType::MultilineComment) {
        this->token_index++;
    }
    return this->tokens[this->token_index++];
}
