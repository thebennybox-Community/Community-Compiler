#ifndef AST_PRETTY_PRINTER_H
#define AST_PRETTY_PRINTER_H

#include <vector>
#include "Ast.h"
#include "TokenStream.h"
#include "Token.h"

void pretty_print_ast(Ast &ast);

void syntax_highlight_print_error(
    const std::string &source, const std::vector<Token> &tokens,
    unsigned int error_line, size_t error_start, size_t error_len,
    size_t context_lines = 5);

void syntax_highlight_print(
    const std::string &source, const std::vector<Token> &tokens);

void syntax_highlight_print(
    const std::string &source, const std::vector<Token> &tokens,
    size_t start, size_t end);

#endif /* AST_PRETTY_PRINTER_H */
