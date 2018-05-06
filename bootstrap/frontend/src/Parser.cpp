#include "Parser.h"

#include <map>

#define cur_tok (this->tokens[this->token_index])
#define peek_tok (this->tokens[this->token_index + 1])

Ast Parser::parse(const std::vector<Token> &tokens) {
    this->tokens = tokens;
    Ast ast;
    ast.root = new AstBlock();

    while(this->token_index < this->tokens.size() - 1) {
        AstNode *statement = parse_stmt();
        if(this->errors.size() == 0 && statement) {
            ast.root->statements.push_back(statement);
        } else {
            if(statement) delete statement;
        }
    }

    return ast;
}

AstNode *Parser::parse_stmt() {
    switch(cur_tok.type) {
    #define _(type, name) case TokenType::type: return parse_##name()

    case TokenType::Symbol: {
        auto result = parse_expr();
        if(!expect(TokenType::SemiColon)) {
            return nullptr;
        }
        return result;
    }
    _(OpenCurlyBracket, block);
    _(StringLiteral, expr);
    _(IntegerLiteral, expr);
    _(FloatLiteral, expr);
    _(Boolean, expr);
    _(Dec, dec);
    _(Fn, fn);
    _(If, if);
    _(Loop, loop);
    _(Continue, continue);
    _(Break, break);
    _(Struct, struct);
    _(Impl, impl);
    _(At, at);
    _(Suffix, affix);
    _(Prefix, affix);
    _(Infix, affix);
    _(Return, return);
    _(Extern, extern);

    case TokenType::MultilineComment:  // Fall through
    case TokenType::SingleLineComment: // Fall through
    case TokenType::SemiColon:         next_token(); return nullptr;

    default:
        next_token();
        this->errors.push_back({ErrorType::UnexpectedToken, cur_tok});
        break;

    #undef _
    }

    return nullptr;
}

AstBlock *Parser::parse_block() {
    if(!expect(TokenType::OpenCurlyBracket)) {
        return nullptr;
    }

    AstBlock *result = new AstBlock();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    while(!accept(TokenType::CloseCurlyBracket)) {
        AstNode *statement = parse_stmt();
        if(!statement) {
            delete result;
            return nullptr;
        }
        result->statements.push_back(statement);
    }

    return result;
}

AstNode *Parser::parse_symbol() {
    if(peek_tok.type == TokenType::OpenParenthesis) {
        // Function call
        AstFnCall *result = new AstFnCall();

        result->line   = cur_tok.line;
        result->column = cur_tok.column;

        result->name = new AstSymbol();
        result->name->name = cur_tok.raw;

        next_token();

        if(!parse_args(result->args)) {
            delete result;
            return nullptr;
        }


        return result;
    } else {

        AstSymbol *result = new AstSymbol();

        result->line   = cur_tok.line;
        result->column = cur_tok.column;
        result->name   = cur_tok.raw;

        next_token();

        return result;
    }
}

AstString *Parser::parse_string() {
    AstString *result = new AstString();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;
    result->value  = cur_tok.raw;


    next_token();

    return result;
}

AstNumber *Parser::parse_number() {
    AstNumber *result = new AstNumber();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    // Extract everything before the suffix, convert to integer, everything
    // after suffix is number of bits. Default integer is i32, default float is
    // f32.

    if(cur_tok.type == TokenType::IntegerLiteral) {
        result->is_float  = false;
        result->is_signed = true;

        size_t suffix_start; // u64, f32, etc

        if((suffix_start = cur_tok.raw.find("u")) != std::string::npos) {
            result->is_signed = false;
            result->value.u = std::stoull(cur_tok.raw.substr(0, suffix_start));
            result->bits = std::stoi(cur_tok.raw.substr(suffix_start + 1));
        } else if((suffix_start = cur_tok.raw.find("i")) != std::string::npos) {
            result->value.i = std::stoll(cur_tok.raw.substr(0, suffix_start));
            result->bits = std::stoi(cur_tok.raw.substr(suffix_start + 1));
        } else {
            result->value.i = std::stoll(cur_tok.raw);
            result->bits = 32;
        }
    } else if(cur_tok.type == TokenType::FloatLiteral) {
        result->is_float = true;

        size_t suffix_start; // u64, f32, etc

        if((suffix_start = cur_tok.raw.find("f")) != std::string::npos) {
            result->value.f = std::stod(cur_tok.raw.substr(0, suffix_start));
            result->bits = std::stoi(cur_tok.raw.substr(suffix_start + 1));
        } else {
            result->value.f = std::stod(cur_tok.raw);
            result->bits = 32;
        }

    }


    next_token();

    return result;
}

AstBoolean *Parser::parse_boolean() {
    AstBoolean *result = new AstBoolean();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;
    result->value  = cur_tok.raw == "true";

    next_token();


    return result;
}

AstArray *Parser::parse_array() {
    AstArray *result = new AstArray();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    next_token();

    while(!accept(TokenType::CloseSquareBracket)) {
        AstNode *element = parse_expr();
        if(!element) {
            delete result;
            return nullptr;
        }
        result->elements.push_back(element);

        if(!accept(TokenType::Comma)) {
            if(!expect(TokenType::CloseSquareBracket)) {
                delete result;
                return nullptr;
            }
            break;
        }
    }

    return result;
}

AstType *Parser::parse_type() {
    AstType *result = new AstType();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;
    result->name   = cur_tok.raw;

    if(!expect(TokenType::Symbol)) {
        delete result;
        return nullptr;
    }

    while(accept(TokenType::OpenSquareBracket)) {
        if(!expect(TokenType::CloseSquareBracket)) {
            delete result;
            return nullptr;
        }
        AstType *new_result = new AstType();
        new_result->line     = result->line;
        new_result->column   = result->column;
        new_result->is_array = true;
        new_result->subtype  = result;
        result = new_result;
    }

    return result;
}

AstDec *Parser::parse_dec() {
    AstDec *result = new AstDec();

    size_t start = this->token_index;

    result->line   = cur_tok.line;
    result->column = cur_tok.column;
    result->immutable = cur_tok.raw == "let";

    next_token();

    result->name = new AstSymbol();
    result->name->name = cur_tok.raw;

    if(!expect(TokenType::Symbol)) {
        delete result;
        return nullptr;
    }

    bool valid = false;

    if(accept(TokenType::Colon)) {
        valid = true;
        result->type = parse_type();
        if(!result->type) {
            delete result;
            return nullptr;
        }
    }

    if(accept(TokenType::Equal)) {
        valid = true;
        result->value = parse_expr();
        if(!result->value) {
            delete result;
            return nullptr;
        }
    }

    if(!valid) {
        this->errors.push_back({ErrorType::InvalidDec, this->tokens[start]});
        delete result;
        return nullptr;
    }

    if(!expect(TokenType::SemiColon)) {
        delete result;
        return nullptr;
    }

    return result;
}

AstIf *Parser::parse_if() {
    AstIf *result = new AstIf();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    next_token();

    if(!expect(TokenType::OpenParenthesis)) {
        delete result;
        return nullptr;
    }
    if(!(result->condition = parse_expr())) {
        delete result;
        return nullptr;
    }
    if(!expect(TokenType::CloseParenthesis)) {
        delete result;
        return nullptr;
    }

    if(!(result->true_block = parse_block())) {
        delete result;
        return nullptr;
    }

    if(accept(TokenType::Else)) {
        if(cur_tok.type == TokenType::If) {
            result->false_block = new AstBlock();
            result->false_block->line   = cur_tok.line;
            result->false_block->column = cur_tok.column;

            AstIf *next_if = parse_if();
            if(!next_if) {
                delete result;
                return nullptr;
            }

            result->false_block->statements.push_back(next_if);
        } else {
            if(!(result->false_block = parse_block())) {
                delete result;
                return nullptr;
            }
        }
    }

    return result;
}

AstFn *Parser::parse_fn(bool require_body) {
    AstFn *result = new AstFn();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    next_token();

    result->name = new AstSymbol();
    result->name->name = cur_tok.raw;

    if(!expect(TokenType::Symbol)) {
        delete result;
        return nullptr;
    }

    if(accept(TokenType::Dot)) {
        result->type_self = result->name;
        result->name = new AstSymbol();
        result->name->name = cur_tok.raw;

        if(!expect(TokenType::Symbol)) {
            delete result;
            return nullptr;
        }

    }

    if(!parse_params(result->params)) {
        delete result;
        return nullptr;
    }

    if(accept(TokenType::Colon)) {
        if(!(result->return_type = parse_type())) {
            delete result;
            return nullptr;
        }
    }

    if(require_body && !(result->body = parse_block())) {
        delete result;
        return nullptr;
    }

    return result;
}

AstLoop *Parser::parse_loop() {
    AstLoop *result = new AstLoop();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;
    result->is_foreach = false;

    next_token();

    if(!expect(TokenType::OpenParenthesis)) {
        delete result;
        return nullptr;
    }

    if(cur_tok.type == TokenType::Symbol && peek_tok.type == TokenType::In) {
        result->is_foreach = true;
        result->name = new AstSymbol();
        result->name->name = cur_tok.raw;

        accept(TokenType::Symbol);
        accept(TokenType::In);
    }

    if(!(result->expr = parse_expr())) {
        delete result;
        return nullptr;
    }

    if(!expect(TokenType::CloseParenthesis)) {
        delete result;
        return nullptr;
    }
    if(!(result->body = parse_block())) {
        delete result;
        return nullptr;
    }

    return result;
}

AstContinue *Parser::parse_continue() {
    AstContinue *result = new AstContinue();
    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    next_token();

    if(!expect(TokenType::SemiColon)) {
        delete result;
        return nullptr;
    }

    return result;
}

AstBreak *Parser::parse_break() {
    AstBreak *result = new AstBreak();
    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    next_token();

    if(!expect(TokenType::SemiColon)) {
        delete result;
        return nullptr;
    }

    return result;
}

AstStruct *Parser::parse_struct() {
    if(!expect(TokenType::Struct)) {
        return nullptr; // Internal error
    }

    AstStruct *result = new AstStruct();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;
    result->block  = new AstBlock();
    result->name   = new AstSymbol();
    result->name->name = cur_tok.raw;

    if(!expect(TokenType::Symbol)) {
        delete result;
        return nullptr;
    }

    if(!expect(TokenType::OpenCurlyBracket)) {
        delete result;
        return nullptr;
    }
    while(!accept(TokenType::CloseCurlyBracket)) {
        AstDec *decl = new AstDec();

        decl->line   = cur_tok.line;
        decl->column = cur_tok.column;
        decl->name   = new AstSymbol();
        decl->name->name = cur_tok.raw;

        if(!expect(TokenType::Symbol)) {
            delete result;
            return nullptr;
        }


        if(!expect(TokenType::Colon)) {
            delete result;
            return nullptr;
        }
        if(!(decl->type = parse_type())) {
            delete result;
            return nullptr;
        }

        result->block->statements.push_back(decl);
    }

    return result;
}

AstImpl *Parser::parse_impl() {
    AstImpl *result = new AstImpl();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    next_token();

    result->name = new AstSymbol();
    result->name->name = cur_tok.raw;

    if(!expect(TokenType::Symbol)) {
        delete result;
        return nullptr;
    }

    if(!(result->block = parse_block())) {
        delete result;
        return nullptr;
    }

    return result;
}

AstAttribute *Parser::parse_at() {
    AstAttribute *result = new AstAttribute();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    next_token();

    result->name = new AstSymbol();
    result->name->name = cur_tok.raw;

    if(!expect(TokenType::Symbol)) {
        delete result;
        return nullptr;
    }

    if(cur_tok.type == TokenType::OpenParenthesis) {
        if(!parse_args(result->args)) {
            delete result;
            return nullptr;
        }
    }

    return result;
}

AstAffix *Parser::parse_affix() {
    static const std::map<std::string, AffixType> affix_types = {
        {"infix",  AffixType::Infix},
        {"prefix", AffixType::Prefix},
        {"suffix", AffixType::Suffix},
    };

    AstAffix *result = new AstAffix();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;
    result->affix_type = affix_types.at(cur_tok.raw);

    next_token();

    if(accept(TokenType::Op)) {
        result->name = new AstSymbol();
        result->name->name = cur_tok.raw;

        if(!expect(TokenType::CustomOperator)) {
            delete result;
            return nullptr;
        }

        if(!parse_params(result->params)) {
            delete result;
            return nullptr;
        }
        if(accept(TokenType::Colon)) {
            if(!(result->return_type = parse_type())) {
                delete result;
                return nullptr;
            }
        }
        if(!(result->body = parse_block())) {
            delete result;
            return nullptr;
        }
    } else if(cur_tok.type == TokenType::Fn) {
        AstFn *fn = parse_fn();
        if(!fn) {
            delete result;
            return nullptr;
        }
        result->name = fn->name;
        result->params = fn->params;
        result->return_type = fn->return_type;
        result->body = fn->body;

        fn->name = nullptr;
        fn->params.clear();
        fn->return_type = nullptr;
        fn->body = nullptr;
    } else {
        delete result;
        return nullptr;
    }

    return result;
}

AstReturn *Parser::parse_return() {
    AstReturn *result = new AstReturn();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    next_token();

    if(accept(TokenType::SemiColon)) {
        return result;
    }

    if(!(result->expr = parse_expr())) {
        delete result;
        return nullptr;
    }

    if(!expect(TokenType::SemiColon)) {
        delete result;
        return nullptr;
    }

    return result;
}

AstExtern *Parser::parse_extern() {
    AstExtern *result = new AstExtern();

    result->line   = cur_tok.line;
    result->column = cur_tok.column;

    next_token();

    if(accept(TokenType::OpenCurlyBracket)) {
        while(!accept(TokenType::CloseCurlyBracket)) {
            AstFn *decl = parse_fn(false);
            if(!decl) {
                delete result;
                return nullptr;
            }
            if(!expect(TokenType::SemiColon)) {
                delete result;
                return nullptr;
            }
            result->decls.push_back(decl);
        }
    } else {
        AstFn *decl = parse_fn(false);
        if(!decl) {
            delete result;
            return nullptr;
        }
        if(!expect(TokenType::SemiColon)) {
            delete result;
            return nullptr;
        }
        result->decls.push_back(decl);
    }

    return result;
}

AstNode *Parser::parse_expr() {
    unsigned int line = cur_tok.line, column = cur_tok.column;

    AstNode *result = parse_expr_primary();
    if(!result) {
        delete result;
        return nullptr;
    }

    if(!token_type_is_operator(cur_tok.type)) {
        return result;
    }

    AstBinaryExpr *expr = new AstBinaryExpr();

    expr->line   = line;
    expr->column = column;
    expr->op = cur_tok.raw;

    next_token();

    expr->lhs = result;
    expr->rhs = parse_expr();

    return expr;
}

AstNode *Parser::parse_expr_primary() {
    AstNode *result;

    switch(cur_tok.type) {
    case TokenType::Symbol:
        result = parse_symbol();
        break;
    case TokenType::StringLiteral:
        result = parse_string();
        break;
    case TokenType::FloatLiteral: // Fall through
    case TokenType::IntegerLiteral:
        result = parse_number();
        break;
    case TokenType::Boolean:
        result = parse_boolean();
        break;
    case TokenType::OpenSquareBracket:
        result = parse_array();
        break;
    case TokenType::OpenParenthesis:
        accept(TokenType::OpenParenthesis);
        result = parse_expr();
        if(!expect(TokenType::CloseParenthesis)) {
            delete result;
            return nullptr;
        }
        break;

    default: return nullptr;
    }

    if(accept(TokenType::OpenSquareBracket)) {
        AstIndex *index = new AstIndex();

        index->line   = result->line;
        index->column = result->column;

        index->array = result;
        index->expr = parse_expr();

        if(!expect(TokenType::CloseSquareBracket)) {
            delete result;
            return nullptr;
        }

        return index;
    }

    return result;
}

bool Parser::parse_params(std::vector<AstDec*> &result) {
    if(!expect(TokenType::OpenParenthesis)) {
        return false;
    }

    while(!accept(TokenType::CloseParenthesis)) {
        AstDec *param = new AstDec();
        param->line   = cur_tok.line;
        param->column = cur_tok.column;

        param->name = new AstSymbol();
        param->name->name = cur_tok.raw;

        if(!expect(TokenType::Symbol)) {
            return false;
        }

        if(!expect(TokenType::Colon)) {
            return false;
        }

        if(!(param->type = parse_type())) {
            return false;
        }

        result.push_back(param);

        if(!accept(TokenType::Comma)) {
            if(!expect(TokenType::CloseParenthesis)) {
                return false;
            }
            break;
        }
    }

    return true;
}

bool Parser::parse_args(std::vector<AstNode*> &result) {
    if(!expect(TokenType::OpenParenthesis)) {
        return false;
    }

    while(!accept(TokenType::CloseParenthesis)) {
        AstNode *expr = parse_expr();
        if(!expr) {
            return false;
        }

        result.push_back(expr);

        if(accept(TokenType::CloseParenthesis)) {
            break;
        }
        if(!expect(TokenType::Comma)) {
            return false;
        }
    }

    return true;
}

bool Parser::next_token() {
    if(this->token_index == this->tokens.size() - 1) {
        return false;
    }
    this->token_index++;

    while(
        this->tokens[this->token_index].type == TokenType::SingleLineComment ||
        this->tokens[this->token_index].type == TokenType::MultilineComment
    ) {
        if(this->token_index == this->tokens.size() - 1) {
            return false;
        }
        this->token_index++;
    }

    return true;
}

bool Parser::accept(TokenType type) {
    if(cur_tok.type == type) {
        next_token();
        return true;
    }

    return false;
}

bool Parser::expect(TokenType type) {
    if(cur_tok.type == type) {
        next_token();
        return true;
    }

    this->errors.push_back({ErrorType::UnexpectedToken, cur_tok});
    return false;
}
