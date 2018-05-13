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
            delete statement;
        }
    }

    return ast;
}

AstNode *Parser::parse_stmt() {
    switch(cur_tok.type) {
    case TokenType::OpenCurlyBracket:  return parse_block();

    case TokenType::Symbol:            // Fall through
    case TokenType::StringLiteral:     // Fall through
    case TokenType::IntegerLiteral:    // Fall through
    case TokenType::HexLiteral:        // Fall through
    case TokenType::FloatLiteral:      // Fall through
    case TokenType::Boolean:           return parse_expr();

    case TokenType::Var:               // Fall through
    case TokenType::Let:               return parse_decl();

    case TokenType::Fn:                return parse_fn();

    case TokenType::If:                return parse_if();

    case TokenType::Loop:              return parse_loop();

    case TokenType::Continue:          return parse_continue();

    case TokenType::Break:             return parse_break();

    case TokenType::Struct:            return parse_struct();

    case TokenType::Impl:              return parse_impl();

    case TokenType::At:                return parse_attr();

    case TokenType::Suffix:            // Fall through
    case TokenType::Prefix:            // Fall through
    case TokenType::Infix:             return parse_affix();

    case TokenType::Return:            return parse_return();

    case TokenType::Extern:            return parse_extern();

    case TokenType::MultilineComment:  // Fall through
    case TokenType::SingleLineComment: // Fall through
    case TokenType::SemiColon:
        next_token();
        return nullptr;

    default:
        this->errors.emplace_back(
            ErrorType::UnexpectedToken, cur_tok,
            "Unexpected token in input"
        );
        next_token();
        break;
    }

    return nullptr;
}

AstBlock *Parser::parse_block() {
    if(!expect(TokenType::OpenCurlyBracket,
               "Expected opening curly bracket at start of block")) {
        return nullptr;
    }

    AstBlock *result = new AstBlock(cur_tok.line, cur_tok.column);

    while(!accept(TokenType::CloseCurlyBracket)) {
        AstNode *statement = parse_stmt();

        if(!statement) {
            delete result;
            return nullptr;
        }

        result->statements.push_back(statement);

        while(accept(TokenType::SemiColon)) {} // Hacky, but works
    }

    return result;
}

AstNode *Parser::parse_symbol() {
    if(peek_tok.type == TokenType::OpenParenthesis) {
        // Function call
        AstFnCall *result = new AstFnCall(cur_tok.line, cur_tok.column);
        result->name = cur_tok.raw;

        next_token();

        if(!parse_args(result->args)) {
            delete result;
            return nullptr;
        }

        return result;
    } else {
        AstSymbol *result = new AstSymbol(cur_tok.line, cur_tok.column);
        result->name = cur_tok.raw;

        next_token();

        return result;
    }
}

AstString *Parser::parse_string() {
    AstString *result = new AstString(cur_tok.line, cur_tok.column);

    result->value = cur_tok.raw;

    next_token();

    return result;
}

AstNumber *Parser::parse_number() {
    AstNumber *result = new AstNumber(cur_tok.line, cur_tok.column);

    // Extract everything before the suffix, convert to integer, everything
    // after suffix is number of bits. Default integer is i32, default float is
    // f32.

    if(cur_tok.type == TokenType::HexLiteral) {
        result->is_float  = false;
        result->is_signed = false;

        size_t suffix_start = cur_tok.raw.find("u");

        if(suffix_start != std::string::npos) {
            const std::string value = cur_tok.raw.substr(0, suffix_start);

            result->value.u = std::stoull(value, 0, 16);
            result->bits    = std::stoi(cur_tok.raw.substr(suffix_start + 1));
        } else {
            result->value.u = std::stoull(cur_tok.raw, 0, 16);
            result->bits    = 32;
        }
    } else if(cur_tok.type == TokenType::IntegerLiteral) {
        result->is_float  = false;
        result->is_signed = true;

        size_t suffix_start; // u64, f32, etc

        if((suffix_start = cur_tok.raw.find("u")) != std::string::npos) {
            result->is_signed = false;

            result->value.u = std::stoull(cur_tok.raw.substr(0, suffix_start));
            result->bits    = std::stoi(cur_tok.raw.substr(suffix_start + 1));
        } else if((suffix_start = cur_tok.raw.find("i")) != std::string::npos) {
            result->value.i = std::stoll(cur_tok.raw.substr(0, suffix_start));
            result->bits    = std::stoi(cur_tok.raw.substr(suffix_start + 1));
        } else {
            result->value.i = std::stoll(cur_tok.raw);
            result->bits    = 32;
        }
    } else if(cur_tok.type == TokenType::FloatLiteral) {
        result->is_float = true;

        size_t suffix_start; // u64, f32, etc

        if((suffix_start = cur_tok.raw.find("f")) != std::string::npos) {
            result->value.f = std::stod(cur_tok.raw.substr(0, suffix_start));
            result->bits    = std::stoi(cur_tok.raw.substr(suffix_start + 1));
        } else {
            result->value.f = std::stod(cur_tok.raw);
            result->bits    = 32;
        }
    }

    next_token();

    return result;
}

AstBoolean *Parser::parse_boolean() {
    AstBoolean *result = new AstBoolean(cur_tok.line, cur_tok.column);

    result->value = cur_tok.raw == "true";

    next_token();

    return result;
}

AstArray *Parser::parse_array() {
    AstArray *result = new AstArray(cur_tok.line, cur_tok.column);

    next_token();

    while(!accept(TokenType::CloseSquareBracket)) {
        AstNode *element = parse_expr();

        if(!element) {
            delete result;
            return nullptr;
        }

        result->elements.push_back(element);

        if(!accept(TokenType::Comma)) {
            if(!expect(TokenType::CloseSquareBracket,
                       "Expected closing square bracket at end of array "
                       "literal")) {
                delete result;
                return nullptr;
            }

            break;
        }
    }

    return result;
}

AstType *Parser::parse_type() {
    AstType *result = new AstType(cur_tok.line, cur_tok.column);

    result->name = cur_tok.raw;

    if(!expect(TokenType::Symbol, "Expected type name")) {
        delete result;
        return nullptr;
    }

    while(accept(TokenType::OpenSquareBracket)) {
        if(!expect(TokenType::CloseSquareBracket,
                   "Expected closing square bracket to match opening square "
                   "bracket in type")) {
            delete result;
            return nullptr;
        }

        AstType *new_result = new AstType(result->line, result->column);

        new_result->is_array = true;
        new_result->subtype  = result;

        result = new_result;
    }

    return result;
}

AstDec *Parser::parse_decl() {
    AstDec *result = new AstDec(cur_tok.line, cur_tok.column);

    size_t start = this->token_index;

    result->immutable = cur_tok.type == TokenType::Let;

    next_token();

    result->name = cur_tok.raw;

    if(!expect(TokenType::Symbol,
               "Expected identifier in declaration")) {
        delete result;
        return nullptr;
    }

    bool valid = false;

    if(accept(TokenType::Colon)) {
        valid        = true;
        result->type = parse_type();

        if(!result->type) {
            delete result;
            return nullptr;
        }
    }

    if(accept(TokenType::Equal)) {
        valid         = true;
        result->value = parse_expr();

        if(!result->value) {
            delete result;
            return nullptr;
        }
    }

    if(!valid) {
        this->errors.emplace_back(
            ErrorType::InvalidDec, this->tokens[start],
            "Invalid declaration, either a type or a value is required"
        );
        delete result;
        return nullptr;
    }

    if(!expect(TokenType::SemiColon,
               "Expected semicolon at end of declaration")) {
        delete result;
        return nullptr;
    }

    return result;
}

AstIf *Parser::parse_if() {
    AstIf *result = new AstIf(cur_tok.line, cur_tok.column);

    next_token();

    if(!expect(TokenType::OpenParenthesis,
               "Expected opening parenthesis after `if`")) {
        delete result;
        return nullptr;
    }

    if(!(result->condition = parse_expr())) {
        delete result;
        return nullptr;
    }

    if(!expect(TokenType::CloseParenthesis,
               "Expected closing parenthesis after if-condition")) {
        delete result;
        return nullptr;
    }

    if(!(result->true_block = parse_block())) {
        delete result;
        return nullptr;
    }

    if(accept(TokenType::Else)) {
        if(cur_tok.type == TokenType::If) {
            result->false_block = new AstBlock(cur_tok.line, cur_tok.column);

            AstIf *next_if = parse_if();

            if(!next_if) {
                delete result;
                return nullptr;
            }

            result->false_block->statements.push_back(next_if);
        } else if(!(result->false_block = parse_block())) {
            delete result;
            return nullptr;
        }
    }

    return result;
}

AstFn *Parser::parse_fn(bool require_body) {
    AstFn *result = new AstFn(cur_tok.line, cur_tok.column);

    next_token();

    result->unmangled_name = cur_tok.raw;
    result->mangled_name   = cur_tok.raw;

    if(!expect(TokenType::Symbol,
               "Expected function or type name after `fn`")) {
        delete result;
        return nullptr;
    }

    if(accept(TokenType::Dot)) {
        result->type_self = result->unmangled_name;

        result->unmangled_name = cur_tok.raw;
        result->mangled_name   = cur_tok.raw;

        if(!expect(TokenType::Symbol,
                   "Expected function name after `.` in function "
                   "declaration")) {
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

    if(!require_body) {
        if(cur_tok.type != TokenType::SemiColon) {
            if(cur_tok.type != TokenType::OpenCurlyBracket) {
                this->errors.emplace_back(
                    ErrorType::UnexpectedToken, cur_tok,
                    "Expected semicolon or opening curly bracket after "
                    "function header"
                );
            } else if(!(result->body = parse_block())) {
                delete result;
                return nullptr;
            }
        }
    } else if(cur_tok.type != TokenType::OpenCurlyBracket) {
        this->errors.emplace_back(
            ErrorType::UnexpectedToken, cur_tok,
            "Expected opening curly bracket at start of function definition"
        );
    } else if(!(result->body = parse_block())) {
        delete result;
        return nullptr;
    }

    return result;
}

AstLoop *Parser::parse_loop() {
    AstLoop *result = new AstLoop(cur_tok.line, cur_tok.column);

    result->is_foreach = false;

    next_token();

    if(!expect(TokenType::OpenParenthesis,
               "Expected opening parenthesis after `loop`")) {
        delete result;
        return nullptr;
    }

    if(cur_tok.type == TokenType::Symbol && peek_tok.type == TokenType::In) {
        result->is_foreach = true;
        result->name = cur_tok.raw;

        accept(TokenType::Symbol);
        accept(TokenType::In);
    }

    if(!(result->expr = parse_expr())) {
        delete result;
        return nullptr;
    }

    if(!expect(TokenType::CloseParenthesis,
               "Expected closing parenthesis after loop expression")) {
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
    AstContinue *result = new AstContinue(cur_tok.line, cur_tok.column);

    next_token();

    if(!expect(TokenType::SemiColon,
               "Expected semicolon after `continue`")) {
        delete result;
        return nullptr;
    }

    return result;
}

AstBreak *Parser::parse_break() {
    AstBreak *result = new AstBreak(cur_tok.line, cur_tok.column);

    next_token();

    if(!expect(TokenType::SemiColon,
               "Expected semicolon after `break`")) {
        delete result;
        return nullptr;
    }

    return result;
}

AstStruct *Parser::parse_struct() {
    if(!expect(TokenType::Struct, "Internal compiler error")) {
        return nullptr; // Internal error
    }

    AstStruct *result = new AstStruct(cur_tok.line, cur_tok.column);
    result->name = cur_tok.raw;

    if(!expect(TokenType::Symbol,
               "Expected struct name after `struct`")) {
        delete result;
        return nullptr;
    }

    result->block = new AstBlock(cur_tok.line, cur_tok.column);

    if(!expect(TokenType::OpenCurlyBracket,
               "Expected opening curly bracket at start of struct")) {
        delete result;
        return nullptr;
    }

    while(!accept(TokenType::CloseCurlyBracket)) {
        AstDec *decl = new AstDec(cur_tok.line, cur_tok.column);
        decl->name = cur_tok.raw;

        if(!expect(TokenType::Symbol,
                   "Expected symbol name at start of declaration in struct")) {
            delete result;
            return nullptr;
        }

        if(!expect(TokenType::Colon,
                   "Expected colon after symbol name in declaration in "
                   "struct")) {
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
    AstImpl *result = new AstImpl(cur_tok.line, cur_tok.column);

    next_token();

    result->name = cur_tok.raw;

    if(!expect(TokenType::Symbol,
               "Expected struct name after `impl`")) {
        delete result;
        return nullptr;
    }

    if(!(result->block = parse_block())) {
        delete result;
        return nullptr;
    }

    return result;
}

AstAttribute *Parser::parse_attr() {
    AstAttribute *result = new AstAttribute(cur_tok.line, cur_tok.column);

    next_token();

    result->name = cur_tok.raw;

    if(!expect(TokenType::Symbol,
               "Expected attribute name after `@`")) {
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

    AstAffix *result = new AstAffix(cur_tok.line, cur_tok.column);

    result->affix_type = affix_types.at(cur_tok.raw);

    next_token();

    if(accept(TokenType::Op)) {
        result->name = cur_tok.raw;

        if(!expect(TokenType::CustomOperator,
                   "Expected operator after `op`")) {
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

        result->name        = fn->unmangled_name;
        result->name        = fn->mangled_name;
        result->params      = fn->params;
        result->return_type = fn->return_type;
        result->body        = fn->body;

        fn->params.clear();
        fn->unmangled_name        = nullptr;
        fn->mangled_name        = nullptr;
        fn->return_type = nullptr;
        fn->body        = nullptr;
    } else {
        delete result;
        return nullptr;
    }

    return result;
}

AstReturn *Parser::parse_return() {
    AstReturn *result = new AstReturn(cur_tok.line, cur_tok.column);

    next_token();

    if(accept(TokenType::SemiColon)) {
        return result;
    }

    if(!(result->expr = parse_expr())) {
        delete result;
        return nullptr;
    }

    if(!expect(TokenType::SemiColon,
               "Expected semicolon after return statement")) {
        delete result;
        return nullptr;
    }

    return result;
}

AstExtern *Parser::parse_extern() {
    AstExtern *result = new AstExtern(cur_tok.line, cur_tok.column);

    next_token();

    if(accept(TokenType::OpenCurlyBracket)) {
        while(!accept(TokenType::CloseCurlyBracket)) {
            AstFn *decl = parse_fn(false);

            if(!decl) {
                delete result;
                return nullptr;
            }

            if(!expect(TokenType::SemiColon,
                       "Expected semicolon after declaration in `extern` "
                       "block")) {
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

        if(!expect(TokenType::SemiColon,
                   "Expected semicolon after `extern` declaration")) {
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

    AstBinaryExpr *expr = new AstBinaryExpr(line, column);

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

    case TokenType::FloatLiteral:
    case TokenType::HexLiteral:
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

        if(!expect(TokenType::CloseParenthesis,
                   "Expected closing parenthesis after parenthesised "
                   "expression")) {
            delete result;
            return nullptr;
        }

        break;

    case TokenType::OpenCurlyBracket:
        result = parse_block();
        break;

    default:
        this->errors.emplace_back(
            ErrorType::UnexpectedToken, cur_tok,
            "Invalid token in primary expression"
        );
        return nullptr;
    }

    if(accept(TokenType::OpenSquareBracket)) {
        AstIndex *index = new AstIndex(result->line, result->column);

        index->array = result;
        index->expr  = parse_expr();

        if(!expect(TokenType::CloseSquareBracket,
                   "Expected closing square bracket after array index "
                   "expression")) {
            delete result;
            return nullptr;
        }

        return index;
    }

    return result;
}

bool Parser::parse_params(std::vector<AstDec *> &result) {
    if(!expect(TokenType::OpenParenthesis,
               "Expected opening parenthesis at start of parameter list")) {
        return false;
    }

    while(!accept(TokenType::CloseParenthesis)) {
        AstDec *param = new AstDec(cur_tok.line, cur_tok.column);
        param->name = cur_tok.raw;

        if(!expect(TokenType::Symbol,
                   "Expected name at start of parameter declaration")) {
            return false;
        }

        if(!expect(TokenType::Colon,
                   "Expected colon after name in parameter declaration")) {
            return false;
        }

        if(!(param->type = parse_type())) {
            return false;
        }

        result.push_back(param);

        if(!accept(TokenType::Comma)) {
            if(!expect(TokenType::CloseParenthesis,
                       "Expected comma or closing parenthesis after "
                       "parameter")) {
                return false;
            }

            break;
        }
    }

    return true;
}

bool Parser::parse_args(std::vector<AstNode *> &result) {
    if(!expect(TokenType::OpenParenthesis,
               "Expected opening parenthesis at start of argument list")) {
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

        if(!expect(TokenType::Comma,
                   "Expected comma or closing parenthesis after argument")) {
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

    while(this->tokens[this->token_index].type == TokenType::SingleLineComment
            || this->tokens[this->token_index].type
            == TokenType::MultilineComment) {
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

bool Parser::expect(TokenType type, std::string message) {
    if(cur_tok.type == type) {
        next_token();
        return true;
    }

    this->errors.emplace_back(
        ErrorType::UnexpectedToken, cur_tok,
        message
    );

    return false;
}
