#include "Parser.h"

#include <stdio.h>

#define cur_tok (this->tokens[this->token_index])

Ast Parser::parse(const std::vector<Token> &tokens) {
    printf("Parser::parse\n");
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
    printf("Parser::parse_stmt\n");
    next_token();

    switch(cur_tok.type) {
        #define _(type, name) case TokenType::type: return parse_##name()

        _(OpenCurlyBracket, block);
        _(Symbol, symbol);
        _(StringLiteral, string);
        _(IntegerLiteral, number);
        _(FloatLiteral, number);
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
            this->errors.push_back({ErrorType::UnexpectedToken, cur_tok});
            break;

        #undef _
    }

    return nullptr;
}

std::unique_ptr<AstBlock> Parser::parse_block() {
    return nullptr; // TODO
}

std::unique_ptr<AstNode> Parser::parse_symbol() {
    return nullptr; // TODO
}

std::unique_ptr<AstString> Parser::parse_string() {
    printf("Parser::parse_string\n");
    std::unique_ptr<AstString> result = std::make_unique<AstString>();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;
    result->value  = cur_tok.raw;

    printf("\"%s\"\n", result->value.c_str());

    return result;
}

std::unique_ptr<AstNumber> Parser::parse_number() {
    printf("Parser::parse_number\n");
    std::unique_ptr<AstNumber> result = std::make_unique<AstNumber>();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    // Extract everything before the suffix, convert to integer, everything
    // after suffix is number of bits. Default integer is i32, default float is
    // f32.

    if(cur_tok.type == TokenType::IntegerLiteral) {
        printf("integer ");
        result->is_float  = false;
        result->is_signed = true;

        size_t suffix_start; // u64, f32, etc

        if((suffix_start = cur_tok.raw.find("u")) != std::string::npos) {
            printf("unsigned %s ", cur_tok.raw.substr(0, suffix_start).c_str());
            result->is_signed = false;
            result->value.u = std::stoull(cur_tok.raw.substr(0, suffix_start));
            result->bits = std::stoi(cur_tok.raw.substr(suffix_start + 1));
            printf("value: %llu ", result->value.u);
        } else if((suffix_start = cur_tok.raw.find("i")) != std::string::npos) {
            printf("signed %s ", cur_tok.raw.substr(0, suffix_start).c_str());
            result->value.i = std::stoll(cur_tok.raw.substr(0, suffix_start));
            result->bits = std::stoi(cur_tok.raw.substr(suffix_start + 1));
            printf("value: %lld ", result->value.i);
        } else {
            printf("signed(default) %s ", cur_tok.raw.c_str());
            result->value.i = std::stoll(cur_tok.raw);
            result->bits = 32;
            printf("value: %lld ", result->value.i);
        }
    } else if(cur_tok.type == TokenType::FloatLiteral) {
        printf("float ");
        result->is_float = true;

        size_t suffix_start; // u64, f32, etc

        if((suffix_start = cur_tok.raw.find("f")) != std::string::npos) {
            result->value.f = std::stod(cur_tok.raw.substr(0, suffix_start));
            result->bits = std::stoi(cur_tok.raw.substr(suffix_start + 1));
        } else {
            result->value.f = std::stod(cur_tok.raw);
            result->bits = 32;
        }

        printf("value: %f ", result->value.f);
    }

    printf("bits: %d\n", result->bits);

    return result;
}

std::unique_ptr<AstBoolean> Parser::parse_boolean() {
    printf("Parser::parse_boolean\n");
    std::unique_ptr<AstBoolean> result = std::make_unique<AstBoolean>();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;
    result->value  = cur_tok.raw == "true";

    printf("value: %s\n", result->value ? "true" : "false");

    return result;
}

std::unique_ptr<AstType> Parser::parse_type() {
    printf("Parser::parse_type\n");
    return nullptr; // TODO
}

std::unique_ptr<AstDec> Parser::parse_dec() {
    printf("Parser::parse_dec\n");
    std::unique_ptr<AstDec> result = std::make_unique<AstDec>();

    size_t start = this->token_index;

    result->line   = cur_tok.line;
    result->column = cur_tok.column;
    result->immutable = cur_tok.raw == "let";

    printf("immutable: %i ", (int)result->immutable);

    if(!expect(TokenType::Symbol)) {
        return nullptr;
    }

    result->name = std::make_unique<AstSymbol>();
    result->name->name = cur_tok.raw;

    bool valid = false;

    if(accept(TokenType::Colon)) {
        printf("colon ");
        valid = true;
        result->type = parse_type();
        if(!result->type) {
            return nullptr;
        }
    }

    if(accept(TokenType::Equal)) {
        printf("equals ");
        valid = true;
        result->value = parse_expr();
        if(!result->value) {
            return nullptr;
        }
    }

    if(!valid) {
        this->errors.push_back({ErrorType::InvalidDec, this->tokens[start]});
    }

    if(!expect(TokenType::SemiColon)) {
        return nullptr;
    }

    printf("name: %s\n", result->name->name.c_str());

    return result;
}

std::unique_ptr<AstFn> Parser::parse_fn() {
    return nullptr; // TODO
}

std::unique_ptr<AstLoop> Parser::parse_loop() {
    return nullptr; // TODO
}

std::unique_ptr<AstContinue> Parser::parse_continue() {
    return nullptr; // TODO
}

std::unique_ptr<AstBreak> Parser::parse_break() {
    return nullptr; // TODO
}

std::unique_ptr<AstGet> Parser::parse_get() {
    return nullptr; // TODO
}

std::unique_ptr<AstSet> Parser::parse_set() {
    return nullptr; // TODO
}

std::unique_ptr<AstImpl> Parser::parse_impl() {
    return nullptr; // TODO
}

std::unique_ptr<AstAttribute> Parser::parse_at() {
    return nullptr; // TODO
}

std::unique_ptr<AstOp> Parser::parse_op() {
    return nullptr; // TODO
}

std::unique_ptr<AstExpr> Parser::parse_expr() {
    return nullptr; // TODO
}

void Parser::next_token() {
    this->token_index++;

    while(
        this->tokens[this->token_index].type == TokenType::SingleLineComment ||
        this->tokens[this->token_index].type == TokenType::MultilineComment
    ) {
        this->token_index++;
    }
}

bool Parser::accept(TokenType type) {
    next_token();
    if(cur_tok.type == type) {
        return true;
    }

    return false;
}

bool Parser::expect(TokenType type) {
    next_token();
    if(cur_tok.type == type) {
        return true;
    }

    this->errors.push_back({ErrorType::UnexpectedToken, cur_tok});
    return false;
}
