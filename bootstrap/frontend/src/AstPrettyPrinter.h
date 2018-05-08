#ifndef AST_PRETTY_PRINTER_H
#define AST_PRETTY_PRINTER_H

#include "Ast.h"
#include "TokenStream.h"

void pretty_print_ast(Ast &ast);

void syntax_highlight_print(
    const std::string &source, const TokenStream &tokens);

#endif /* AST_PRETTY_PRINTER_H */
