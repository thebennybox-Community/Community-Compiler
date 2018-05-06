#include "AstPrettyPrinter.h"

#include <stdio.h>
#include <string>

#define INDENT_CHARS "  "

namespace TermColour {
    enum Name {
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        Grey,
    };
}

static constexpr const char *term_fg[] = {
    "\x1B[30m", "\x1B[31m", "\x1B[32m", "\x1B[33m",
    "\x1B[34m", "\x1B[35m", "\x1B[36m", "\x1B[37m",
    "\x1B[90m",
};

static constexpr const char *term_bg[] = {
    "\x1B[40m", "\x1B[41m", "\x1B[42m", "\x1B[43m",
    "\x1B[44m", "\x1B[45m", "\x1B[46m", "\x1B[47m",
    "",
};

static constexpr const char *term_reset     = "\x1B[0m";
static constexpr const char *term_bold      = "\x1B[1m";
static constexpr const char *term_dim       = "\x1B[2m";
static constexpr const char *term_underline = "\x1B[4m";
static constexpr const char *term_reverse   = "\x1B[7m";

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
    switch(node->type) {
    case AstNodeType::AstBlock:
        pretty_print_block((AstBlock*)node, indent);
        break;
    case AstNodeType::AstString:
        pretty_print_string((AstString*)node, indent);
        break;
    case AstNodeType::AstNumber:
        pretty_print_number((AstNumber*)node, indent);
        break;
    case AstNodeType::AstBoolean:
        pretty_print_bool((AstBoolean*)node, indent);
        break;
    case AstNodeType::AstArray:
        pretty_print_array((AstArray*)node, indent);
        break;
    case AstNodeType::AstDec:
        pretty_print_dec((AstDec*)node, indent);
        break;
    case AstNodeType::AstIf:
        pretty_print_if((AstIf*)node, indent);
        break;
    case AstNodeType::AstFn:
        pretty_print_fn((AstFn*)node, indent);
        break;
    case AstNodeType::AstFnCall:
        pretty_print_fn_call((AstFnCall*)node, indent);
        break;
    case AstNodeType::AstLoop:
        pretty_print_loop((AstLoop*)node, indent);
        break;
    case AstNodeType::AstContinue:
        pretty_print_continue((AstContinue*)node, indent);
        break;
    case AstNodeType::AstBreak:
        pretty_print_break((AstBreak*)node, indent);
        break;
    case AstNodeType::AstStruct:
        pretty_print_struct((AstStruct*)node, indent);
        break;
    case AstNodeType::AstImpl:
        pretty_print_impl((AstImpl*)node, indent);
        break;
    case AstNodeType::AstAttribute:
        pretty_print_attribute((AstAttribute*)node, indent);
        break;
    case AstNodeType::AstAffix:
        pretty_print_affix((AstAffix*)node, indent);
        break;
    case AstNodeType::AstUnaryExpr:
        pretty_print_unary((AstUnaryExpr*)node, indent);
        break;
    case AstNodeType::AstBinaryExpr:
        pretty_print_binary((AstBinaryExpr*)node, indent);
        break;
    case AstNodeType::AstIndex:
        pretty_print_index((AstIndex*)node, indent);
        break;
    case AstNodeType::AstType:
        pretty_print_type((AstType*)node, indent);
        break;
    case AstNodeType::AstSymbol:
        pretty_print_symbol((AstSymbol*)node, indent);
        break;
    case AstNodeType::AstReturn:
        pretty_print_return((AstReturn*)node, indent);
        break;
    case AstNodeType::AstExtern:
        pretty_print_extern((AstExtern*)node, indent);
        break;
    default: printf("Uh what\n"); break;
    }
}

void pretty_print_block(const AstBlock *node, std::string indent) {
    printf(
        "%s%sblock%s - %zu statements\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset,
        node->statements.size()
    );
    for(unsigned int i = 0; i < node->statements.size(); i++) {
        pretty_print_node(node->statements[i], indent + INDENT_CHARS);
    }
}

void pretty_print_string(const AstString *node, std::string indent) {
    printf(
        "%s%sstring%s - \"%s\"\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset,
        node->value.c_str()
    );
}

void pretty_print_number(const AstNumber *node, std::string indent) {
    printf(
        "%s%snumber%s - %s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset,
        std::to_string(
            node->is_float ? node->value.f :
            node->is_signed ? node->value.i :
            node->value.u
        ).c_str()
    );
}

void pretty_print_bool(const AstBoolean *node, std::string indent) {
    printf(
        "%s%sboolean%s - %s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset,
        node->value ? "true" : "false"
    );
}

void pretty_print_array(const AstArray *node, std::string indent) {
    printf(
        "%s%sarray%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
}

void pretty_print_dec(const AstDec *node, std::string indent) {
    printf(
        "%s%sdec%s - immutable:%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset,
        node->immutable ? "true" : "false"
    );
    pretty_print_symbol(node->name, indent + INDENT_CHARS);
    if(node->type) {
        pretty_print_type(node->type, indent + INDENT_CHARS);
    }
    if(node->value) {
        pretty_print_node(node->value, indent + INDENT_CHARS);
    }
}

void pretty_print_if(const AstIf *node, std::string indent) {
    printf(
        "%s%sif%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
    pretty_print_node(node->condition, indent + INDENT_CHARS);
    pretty_print_node(node->true_block, indent + INDENT_CHARS);
    if(node->false_block) {
        pretty_print_node(node->false_block, indent + INDENT_CHARS);
    }
}

void pretty_print_fn(const AstFn *node, std::string indent) {
    printf(
        "%s%sfn%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
    pretty_print_symbol(node->name, indent + INDENT_CHARS);
    if(node->return_type) {
        pretty_print_type(node->return_type, indent + INDENT_CHARS);
    }
    for(auto param : node->params) {
        printf(
            "%s%sparam%s - %s\n",
            (indent + INDENT_CHARS).c_str(),
            term_fg[TermColour::Yellow], term_reset,
            param->name->name.c_str()
        );
    }
    if(node->body) {
        pretty_print_block(node->body, indent + INDENT_CHARS);
    }
}

void pretty_print_fn_call(const AstFnCall *node, std::string indent) {
    printf(
        "%s%sfn call%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
    pretty_print_symbol(node->name, indent + INDENT_CHARS);
    for(auto expr : node->args) {
        pretty_print_node(expr, indent + INDENT_CHARS);
    }
}

void pretty_print_loop(const AstLoop *node, std::string indent) {
    printf(
        "%s%sloop%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
    pretty_print_node(node->expr, indent + INDENT_CHARS);
    pretty_print_block(node->body, indent + INDENT_CHARS);
}

void pretty_print_continue(const AstContinue *node, std::string indent) {
    printf(
        "%s%scontinue%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
}

void pretty_print_break(const AstBreak *node, std::string indent) {
    printf(
        "%s%sbreak%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
}

void pretty_print_struct(const AstStruct *node, std::string indent) {
    printf(
        "%s%sstruct%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
    pretty_print_block(node->block, indent + INDENT_CHARS);
}

void pretty_print_impl(const AstImpl *node, std::string indent) {
    printf(
        "%s%simpl%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
    pretty_print_symbol(node->name, indent + INDENT_CHARS);
    pretty_print_block(node->block, indent + INDENT_CHARS);
}

void pretty_print_attribute(const AstAttribute *node, std::string indent) {
    printf(
        "%s%sattribute%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
    pretty_print_symbol(node->name, indent + INDENT_CHARS);
}

void pretty_print_affix(const AstAffix *node, std::string indent) {
    printf(
        "%s%saffix%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
    if(node->return_type) {
        pretty_print_type(node->return_type, indent + INDENT_CHARS);
    }
    pretty_print_symbol(node->name, indent + INDENT_CHARS);
    for(auto param : node->params) {
        printf(
            "%s%sparam%s - %s\n",
            (indent + INDENT_CHARS).c_str(),
            term_fg[TermColour::Yellow], term_reset,
            param->name->name.c_str()
        );
    }
}

void pretty_print_unary(const AstUnaryExpr *node, std::string indent) {
    printf(
        "%s%sunary expr%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
}

void pretty_print_binary(const AstBinaryExpr *node, std::string indent) {
    printf(
        "%s%sbinary expr%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
}

void pretty_print_index(const AstIndex *node, std::string indent) {
    printf(
        "%s%sindex%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
}

void pretty_print_type(const AstType *node, std::string indent) {
    printf(
        "%s%stype%s",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );

    if(!node->is_array) {
        printf(" - %s\n", node->name.c_str());
    } else {
        printf(
            "\n%s%sarray%s\n",
            (indent + INDENT_CHARS).c_str(),
            term_fg[TermColour::Yellow], term_reset
        );
        pretty_print_type(node->subtype, indent + INDENT_CHARS);
    }
}

void pretty_print_symbol(const AstSymbol *node, std::string indent) {
    printf(
        "%s%ssymbol%s - %s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset,
        node->name.c_str()
    );
}

void pretty_print_return(const AstReturn *node, std::string indent) {
    printf(
        "%s%sreturn%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
    if(node->expr) {
        pretty_print_node(node->expr, indent + INDENT_CHARS);
    }
}

void pretty_print_extern(const AstExtern *node, std::string indent) {
    printf(
        "%s%sextern%s\n",
        indent.c_str(),
        term_fg[TermColour::Yellow], term_reset
    );
}

void pretty_print_ast(const Ast &ast) {
    pretty_print_block(ast.root, "");
}
