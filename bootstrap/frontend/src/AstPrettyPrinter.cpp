#include "AstPrettyPrinter.h"

#include <inttypes.h>
#include <stdio.h>
#include <string>
#include "Token.h"
#include "Terminal.h"

#define INDENT_CHARS "  "

static constexpr const char *const code_bg_esc_seq = "\x1B[48;5;236m";

static std::string type_to_string(const AstType *node) {
    if(node->is_array) {
        return type_to_string(node->subtype) + "[]";
    }

    return node->name;
}

void pretty_print_block(const AstBlock *node, std::string indent);
void pretty_print_string(const AstString *node, std::string indent);
void pretty_print_number(const AstNumber *node, std::string indent);
void pretty_print_bool(const AstBoolean *node, std::string indent);
void pretty_print_array(const AstArray *node, std::string indent);
void pretty_print_dec(const AstDec *node, std::string indent);
void pretty_print_if(const AstIf *node, std::string indent);
void pretty_print_fn(const AstFn *node, std::string indent);
void pretty_print_fn_call(const AstFnCall *node, std::string indent);
void pretty_print_loop(const AstLoop *node, std::string indent);
void pretty_print_continue(const AstContinue *node, std::string indent);
void pretty_print_break(const AstBreak *node, std::string indent);
void pretty_print_struct(const AstStruct *node, std::string indent);
void pretty_print_impl(const AstImpl *node, std::string indent);
void pretty_print_attribute(const AstAttribute *node, std::string indent);
void pretty_print_affix(const AstAffix *node, std::string indent);
void pretty_print_unary(const AstUnaryExpr *node, std::string indent);
void pretty_print_binary(const AstBinaryExpr *node, std::string indent);
void pretty_print_index(const AstIndex *node, std::string indent);
void pretty_print_type(const AstType *node, std::string indent);
void pretty_print_symbol(const AstSymbol *node, std::string indent);
void pretty_print_return(const AstReturn *node, std::string indent);
void pretty_print_extern(const AstExtern *node, std::string indent);

void pretty_print_node(const AstNode *node, std::string indent) {
    switch(node->node_type) {
    case AstNodeType::AstBlock:
        pretty_print_block((const AstBlock *)node, indent);
        break;

    case AstNodeType::AstString:
        pretty_print_string((const AstString *)node, indent);
        break;

    case AstNodeType::AstNumber:
        pretty_print_number((const AstNumber *)node, indent);
        break;

    case AstNodeType::AstBoolean:
        pretty_print_bool((const AstBoolean *)node, indent);
        break;

    case AstNodeType::AstArray:
        pretty_print_array((const AstArray *)node, indent);
        break;

    case AstNodeType::AstDec:
        pretty_print_dec((const AstDec *)node, indent);
        break;

    case AstNodeType::AstIf:
        pretty_print_if((const AstIf *)node, indent);
        break;

    case AstNodeType::AstFn:
        pretty_print_fn((const AstFn *)node, indent);
        break;

    case AstNodeType::AstFnCall:
        pretty_print_fn_call((const AstFnCall *)node, indent);
        break;

    case AstNodeType::AstLoop:
        pretty_print_loop((const AstLoop *)node, indent);
        break;

    case AstNodeType::AstContinue:
        pretty_print_continue((const AstContinue *)node, indent);
        break;

    case AstNodeType::AstBreak:
        pretty_print_break((const AstBreak *)node, indent);
        break;

    case AstNodeType::AstStruct:
        pretty_print_struct((const AstStruct *)node, indent);
        break;

    case AstNodeType::AstImpl:
        pretty_print_impl((const AstImpl *)node, indent);
        break;

    case AstNodeType::AstAttribute:
        pretty_print_attribute((const AstAttribute *)node, indent);
        break;

    case AstNodeType::AstAffix:
        pretty_print_affix((const AstAffix *)node, indent);
        break;

    case AstNodeType::AstUnaryExpr:
        pretty_print_unary((const AstUnaryExpr *)node, indent);
        break;

    case AstNodeType::AstBinaryExpr:
        pretty_print_binary((const AstBinaryExpr *)node, indent);
        break;

    case AstNodeType::AstIndex:
        pretty_print_index((const AstIndex *)node, indent);
        break;

    case AstNodeType::AstType:
        pretty_print_type((const AstType *)node, indent);
        break;

    case AstNodeType::AstSymbol:
        pretty_print_symbol((const AstSymbol *)node, indent);
        break;

    case AstNodeType::AstReturn:
        pretty_print_return((const AstReturn *)node, indent);
        break;

    case AstNodeType::AstExtern:
        pretty_print_extern((const AstExtern *)node, indent);
        break;

    default:
        printf("Uh what\n");
        break;
    }
}

void pretty_print_block(const AstBlock *node, std::string indent) {
    printf(
        "%s%sblock%s %s%s(%zu statements)%s %s{%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset,
        term_dim,
        term_fg[TermColour::White],
        node->statements.size(),
        term_reset,
        term_fg[TermColour::Grey],
        term_reset);

    for(unsigned int i = 0; i < node->statements.size(); i++) {
        pretty_print_node(node->statements[i], indent + INDENT_CHARS);
    }

    printf("%s%s}%s\n", indent.c_str(), term_fg[TermColour::Grey], term_reset);
}

void pretty_print_string(const AstString *node, std::string indent) {
    printf(
        "%s%sstring%s - \"%s\"\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset,
        node->value.c_str());
}

void pretty_print_number(const AstNumber *node, std::string indent) {
    printf(
        "%s%snumber%s - ",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);

    if(node->is_float) {
        printf("%f\n", node->value.f);
    } else if(node->is_signed) {
        printf("%" PRId64 "\n", node->value.i);
    } else {
        printf("%" PRIu64 "\n", node->value.u);
    }
}

void pretty_print_bool(const AstBoolean *node, std::string indent) {
    printf(
        "%s%sboolean%s - %s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset,
        node->value ? "true" : "false");
}

void pretty_print_array(const AstArray *node, std::string indent) {
    printf(
        "%s%sarray%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);

    for(auto element : node->elements) {
        pretty_print_node(element, indent + INDENT_CHARS);
    }
}

void pretty_print_dec(const AstDec *node, std::string indent) {
    printf(
        "%s%s%s%s %s%s%s",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        node->immutable ? "let" : "var",
        term_reset,
        term_fg[TermColour::Red],
        node->name.c_str(),
        term_reset);

    if(node->type) {
        printf(
            ": %s%s%s",
            term_fg[TermColour::Magenta],
            type_to_string(node->type).c_str(),
            term_reset);
    }

    printf("\n");

    if(node->value) {
        pretty_print_node(node->value, indent + INDENT_CHARS);
    }
}

void pretty_print_if(const AstIf *node, std::string indent) {
    printf(
        "%s%sif%s\n", indent.c_str(), term_fg[TermColour::Yellow], term_reset);
    pretty_print_node(node->condition, indent + INDENT_CHARS);
    pretty_print_node(node->true_block, indent + INDENT_CHARS);

    if(node->false_block) {
        pretty_print_node(node->false_block, indent + INDENT_CHARS);
    }
}

void pretty_print_fn(const AstFn *node, std::string indent) {
    printf(
        "%s%sfn%s %s%s%s",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset,
        term_fg[TermColour::Blue],
        node->mangled_name.c_str(),
        term_reset);

    if(node->return_type) {
        printf(
            ": %s%s%s",
            term_fg[TermColour::Magenta],
            type_to_string(node->return_type).c_str(),
            term_reset);
    }

    printf("\n");

    for(auto param : node->params) {
        printf(
            "%s%sparam%s - %s\n",
            (indent + INDENT_CHARS).c_str(),
            term_fg[TermColour::Yellow],
            term_reset,
            param->name.c_str());
    }

    if(node->body) {
        pretty_print_block(node->body, indent + INDENT_CHARS);
    }
}

void pretty_print_fn_call(const AstFnCall *node, std::string indent) {
    printf(
        "%s%sfn call%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);
    //pretty_print_symbol(node->name, indent + INDENT_CHARS);

    for(auto expr : node->args) {
        pretty_print_node(expr, indent + INDENT_CHARS);
    }
}

void pretty_print_loop(const AstLoop *node, std::string indent) {
    printf(
        "%s%sloop%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);
    pretty_print_node(node->expr, indent + INDENT_CHARS);
    pretty_print_block(node->body, indent + INDENT_CHARS);
}

void pretty_print_continue(const AstContinue *node, std::string indent) {
    (void)node;

    printf(
        "%s%scontinue%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);
}

void pretty_print_break(const AstBreak *node, std::string indent) {
    (void)node;

    printf(
        "%s%sbreak%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);
}

void pretty_print_struct(const AstStruct *node, std::string indent) {
    printf(
        "%s%sstruct%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);
    pretty_print_block(node->block, indent + INDENT_CHARS);
}

void pretty_print_impl(const AstImpl *node, std::string indent) {
    printf(
        "%s%simpl%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);
    //pretty_print_symbol(node->name, indent + INDENT_CHARS);
    pretty_print_block(node->block, indent + INDENT_CHARS);
}

void pretty_print_attribute(const AstAttribute *node, std::string indent) {
    printf(
        "%s%sattribute%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);
    //pretty_print_symbol(node->name, indent + INDENT_CHARS);
}

void pretty_print_affix(const AstAffix *node, std::string indent) {
    if(node->affix_type == AffixType::Prefix) {
        printf(
            "%s%sprefix%s\n",
            indent.c_str(),
            term_fg[TermColour::Yellow],
            term_reset);
    } else if(node->affix_type == AffixType::Infix) {
        printf(
            "%s%sinfix%s\n",
            indent.c_str(),
            term_fg[TermColour::Yellow],
            term_reset);
    } else {
        printf(
            "%s%ssuffix%s\n",
            indent.c_str(),
            term_fg[TermColour::Yellow],
            term_reset);
    }

    if(node->return_type) {
        pretty_print_type(node->return_type, indent + INDENT_CHARS);
    }

    //pretty_print_symbol(node->name, indent + INDENT_CHARS);

    for(auto param : node->params) {
        printf(
            "%s%sparam%s - %s\n",
            (indent + INDENT_CHARS).c_str(),
            term_fg[TermColour::Yellow],
            term_reset,
            param->name.c_str());
    }

    pretty_print_block(node->body, indent + INDENT_CHARS);
}

void pretty_print_unary(const AstUnaryExpr *node, std::string indent) {
    (void)node;

    printf(
        "%s%sunary expr%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);
}

void pretty_print_binary(const AstBinaryExpr *node, std::string indent) {
    printf(
        "%s%s%s (binary expr)%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        node->op.c_str(),
        term_reset);
    pretty_print_node(node->lhs, indent + INDENT_CHARS);

    if(node->rhs) {
        pretty_print_node(node->rhs, indent + INDENT_CHARS);
    }
}

void pretty_print_index(const AstIndex *node, std::string indent) {
    printf(
        "%s%sindex%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);
    pretty_print_node(node->array, indent + INDENT_CHARS);
    pretty_print_node(node->expr, indent + INDENT_CHARS);
}

void pretty_print_type(const AstType *node, std::string indent) {
    printf(
        "%s%stype%s", indent.c_str(), term_fg[TermColour::Yellow], term_reset);

    if(!node->is_array) {
        printf(" - %s\n", node->name.c_str());
    } else {
        printf(
            "\n%s%sarray%s\n",
            (indent + INDENT_CHARS).c_str(),
            term_fg[TermColour::Yellow],
            term_reset);
        pretty_print_type(node->subtype, indent + INDENT_CHARS);
    }
}

void pretty_print_symbol(const AstSymbol *node, std::string indent) {
    printf(
        "%s%ssymbol%s - %s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset,
        node->name.c_str());
}

void pretty_print_return(const AstReturn *node, std::string indent) {
    printf(
        "%s%sreturn%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);

    if(node->expr) {
        pretty_print_node(node->expr, indent + INDENT_CHARS);
    }
}

void pretty_print_extern(const AstExtern *node, std::string indent) {
    printf(
        "%s%sextern%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow],
        term_reset);

    for(auto decl : node->decls) {
        pretty_print_fn(decl, indent + INDENT_CHARS);
    }
}

void pretty_print_ast(Ast &ast) {
    pretty_print_block(ast.root, "");
}

static void set_colour(size_t i, const TokenStream &tokens) {
    for(size_t j = 0; j < tokens.tokens.size(); j++) {
        const auto &token = tokens.tokens[j];

        if(token.offset <= i && token.offset + token.raw.size() > i) {
            switch(token.type) {
            case TokenType::If:
            case TokenType::Else:
            case TokenType::Continue:
            case TokenType::Break:
            case TokenType::Loop:
            case TokenType::In:
            case TokenType::Fn:
            case TokenType::Op:
            case TokenType::Infix:
            case TokenType::Prefix:
            case TokenType::Suffix:
            case TokenType::Extern:
            case TokenType::Struct:
            case TokenType::Impl:
            case TokenType::Var:
            case TokenType::Let:
            case TokenType::Return:
                printf("%s", term_fg[TermColour::Magenta]);
                break;

            case TokenType::IntegerLiteral:
            case TokenType::HexLiteral:
            case TokenType::FloatLiteral:
            case TokenType::StringLiteral:
            case TokenType::Boolean:
                printf("%s", term_fg[TermColour::Green]);
                break;

            case TokenType::SingleLineComment:
            case TokenType::MultilineComment:
                printf("%s", term_fg[TermColour::Grey]);
                break;

            case TokenType::Symbol:
                if(tokens.tokens[j + 1].type == TokenType::OpenParenthesis) {
                    // Function
                    printf("%s", term_fg[TermColour::Blue]);
                } else if(tokens.tokens[j - 1].type == TokenType::Colon) {
                    // Type
                    printf("%s", term_fg[TermColour::Red]);
                } else {
                    printf("%s", term_reset);
                }

                break;

            default:
                printf("%s", term_reset);
                break;
            }
        }
    }
}

void syntax_highlight_print_line(
    const std::string &source, const TokenStream &tokens,
    size_t error_start, size_t error_len, size_t context_lines
) {
    int lines, columns;
    get_term_size(&lines, &columns);

    size_t i = error_start, end = error_start;

    for(size_t j = 0; j <= (context_lines - 1) / 2; j++) {
        while(i > 0 && source[i] != '\n') {
            i--;
        }
        if(i == 0) {
            break;
        }
        i--;
    }
    if(i != 0) {
        i += 2;
    }

    for(size_t j = 0; j <= (context_lines - 1) / 2; j++) {
        while(end < source.size() && source[end] != '\n') {
            end++;
        }
        if(end == source.size()) {
            break;
        }
        end++;
    }
    if(end != source.size()) {
        end--;
    }

    int column = 0;

    for(; i < end; i++) {
        column++;

        if(i < error_start || i > error_start + error_len - 1) {
            set_colour(i, tokens);
            printf("%s", code_bg_esc_seq);
            if(source[i] == '\n') {
                putchar(' ');
            } else {
                putchar(source[i]);
            }
            printf("%s", term_reset);
        } else {
            printf("%s%s",
                   term_bg[TermColour::Red], term_fg[TermColour::Black]);
            if(source[i] == '\n') {
                printf(" %s", term_reset);
            } else {
                putchar(source[i]);
            }
            printf("%s", term_reset);
        }

        if(source[i] == '\n') {
            printf("%s", code_bg_esc_seq);
            while(column < columns) {
                putchar(' ');
                column++;
            }
            column = 0;
            putchar('\n');
        }

        printf("%s", term_reset);
    }

    printf(code_bg_esc_seq);
    while(column < columns) {
        putchar(' ');
        column++;
    }
    printf("%s\n", term_reset);
}

void syntax_highlight_print(
    const std::string &source, const TokenStream &tokens) {
    syntax_highlight_print(source, tokens, 0, source.size());
}

void syntax_highlight_print(
    const std::string &source, const TokenStream &tokens,
    size_t start, size_t end
) {
    for(size_t i = start; i < end; i++) {
        set_colour(i, tokens);
        putchar(source[i]);
    }
}
