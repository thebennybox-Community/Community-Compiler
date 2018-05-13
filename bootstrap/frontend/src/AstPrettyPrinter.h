#ifndef AST_PRETTY_PRINTER_H
#define AST_PRETTY_PRINTER_H

#include "Ast.h"
#include "TokenStream.h"

void pretty_print_ast(Ast &ast);

void syntax_highlight_print_line(
    const std::string &source, const TokenStream &tokens,
    size_t i, const Token &error_token, size_t context_lines = 5);

void syntax_highlight_print(
    const std::string &source, const TokenStream &tokens);

void syntax_highlight_print(
    const std::string &source, const TokenStream &tokens,
    size_t start, size_t end);

#endif /* AST_PRETTY_PRINTER_H */
