#ifndef PARSER_H
#define PARSER_H

#include "Ast.h"
#include "Error.h"
#include "Token.h"
#include <cstddef>
#include <vector>

class Parser {
public:
    /**
     * Parses a list of tokens in to an AST.
     *
     * @param tokens The list of tokens to parse.
     *
     * @return Ast object, containing the root block
     */
    Ast parse(const std::vector<Token> &tokens);

    /** List of errors that occurred during parsing */
    std::vector<Error> errors;

private:
    /**
     * Parses a single statement.
     *
     * @return The statement node
     */
    AstNode *parse_stmt();

    /**
     * Parses a block. Expects the current token to be an opening curly bracket.
     * After this function the current token will be the one after the matching
     * closing curly bracket.
     *
     * @return The block node
     */
    AstBlock *parse_block();

    /**
     * Parses an expression starting with a symbol. This can be a symbol on its
     * own or a function call.
     *
     * @return A symbol or function call node
     */
    AstNode *parse_symbol();

    /**
     * Parses a string.
     *
     * @return The string node
     */
    AstString *parse_string();

    /**
     * Parses a number, converting the string representation to a float or int
     * as appropriate.
     *
     * @return The number node
     */
    AstNumber *parse_number();

    /**
     * Parses a boolean literal (true or false).
     *
     * @return The boolean node
     */
    AstBoolean *parse_boolean();

    /**
     * Parses an array literal. Expects the current token to be the opening
     * square bracket. After this function the current token will be the one
     * after the matching closing square bracket.
     *
     * @return The array node
     */
    AstArray *parse_array();

    /**
     * Parses a type.
     *
     * @return The type node
     */
    AstType *parse_type();

    /**
     * Parses a declaration. Expects the current token to be either "var" or
     * "let", and the statemnt should end with a semicolon. After this function,
     * the current token will be the one after the semicolon.
     *
     * @return The declaration node
     */
    AstDec *parse_decl();

    /**
     * Parses a function declaration or definition. Expects the current token to
     * be "fn". After this function the current token will be either:
     * - The one after the closing curly bracket, if there is a body.
     * - The one after the semicolon, if there is not a body.
     *
     * @param require_body Whether to expect a function body.
     *
     * @return The function node
     */
    AstFn *parse_fn(bool require_body = true);

    /**
     * Parses a loop.
     *
     * @return The loop node
     */
    AstLoop *parse_loop();

    /**
     * Parses an if statement, including the chain of else if/else statements.
     * Expects the current token to be "if". After this function, the current
     * token is the one after the closing curly bracket.
     *
     * @return The if node
     */
    AstIf *parse_if();

    /**
     * Parses a continue statement.
     *
     * @return The continue node
     */
    AstContinue *parse_continue();

    /**
     * Parses a break statement.
     *
     * @return The break node
     */
    AstBreak *parse_break();

    /**
     * Parses a struct declaration. Expects the current token to be "struct".
     * After this function, the current token is the one after the closing
     * curly bracket.
     *
     * @return The struct node
     */
    AstStruct *parse_struct();

    /**
     * Parses an impl block. Expects the current token to be "impl". After this
     * function, the current token is the one after the closing curly bracket.
     *
     * @return [description]
     */
    AstImpl *parse_impl();

    /**
     * Parses an attribute.
     *
     * @return The attribute node
     */
    AstAttribute *parse_attr();

    /**
     * Parses an affix function or operator. Expects the current token to be
     * "infix", "prefix" or "suffix". After this function, the current token is
     * the one
 after the closing curly bracket.
     *
     * @return The affix node
     */
    AstAffix *parse_affix();

    /**
     * Parses a return statment with an optional expression. Expects the current
     * token to be "return". After this function the current token is the one
     * after the semicolon.
     *
     * @return The return node
     */
    AstReturn *parse_return();

    /**
     * Parses an extern block or declaration. Expects the current token to be
     * "extern". After this function, the current token is the one after either:
     * - The closing curly bracket, if it is an extern block.
     * - The semicolon, if it is a single declaration.
     *
     * @return The extern node
     */
    AstExtern *parse_extern();

    /**
     * Parses an expression.
     *
     * @return The expression node
     */
    AstNode *parse_expr();

    /**
     * Parses a primary expression. This is anything starting with a symbol, an
     * unary operator, a literal, or a parenthesised expression, and includes
     * array indexing after.
     *
     * @return The expression node
     */
    AstNode *parse_expr_primary();

    /**
     * Parses a parameter list. Expects the current token to be the opening
     * parenthesis. After this function, the current token is the one after the
     * matching closing parenthesis.
     *
     * @param result The vector to push the paramater declarations to.
     *
     * @return Whether the list was parsed successfully
     */
    bool parse_params(std::vector<AstDec *> &result);

    /**
     * Parses an argument list. Expects the current token to be the opening
     * parenthesis. After this function, the current token is the one after the
     * matching closing parenthesis.
     *
     * @param result The vector to push the argument values to.
     *
     * @return Whether the list was parsed successfully
     */
    bool parse_args(std::vector<AstNode *> &result);

    /**
     * Advances to the next token, not including comments.
     *
     * @return fales if there are no tokens left in the stream, else true
     */
    bool next_token();

    /**
     * Eats the current token if it matches the given type.
     *
     * @param type The type to match.
     *
     * @return Whether the token was accepted
     */
    bool accept(TokenType type);

    /**
     * Eats the current token if it matches the given type. If it doesn't match,
     * adds an error with type UnexpectedToken.
     *
     * @param type The type to match.
     *
     * @return Whether the token was accepted
     */
    bool expect(TokenType type, std::string message);

    /** Local copy of the token stream */
    std::vector<Token> tokens;

    /** The index in the token stream of the current token */
    size_t token_index = 0;
};

#endif /* PARSER_H */
