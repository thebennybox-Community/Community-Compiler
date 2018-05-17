#ifndef DUSKASSEMBLY_H
#define DUSKASSEMBLY_H

#include <string>
#include <vector>
#include <stdint.h>
#include "AstDefs.h"
#include "ScopeContext.h"
#include "Error.h"
#include "Token.h"
#include "ILemitter.h"

class DuskAssembly {
public:
    std::vector<ScopeContext *> scopes;
    ILemitter il_emitter;
    ScopeContext *find_scope(AstNamespace *ns);
    void queue_file(std::string filename);
    void compile_write_binary(std::string out_file);

private:
    Ast parse_file(std::string filename);
    bool semantic_analysis(Ast &ast, int pass);
    bool semantic_analyse_node(AstNode *node, int pass);
    bool semantic_generation(Ast &ast, int pass);
    bool semantic_generation_node(AstNode *node, int pass);
    bool generate_code(Ast &ast);
    bool generate_code_node(AstNode *node);
    bool handle_errors(std::vector<Error> errors,
                       std::string file_contents, std::vector<Token> tokens);

    std::vector<std::string> queued_files;
};

#endif /* DUSKASSEMBLY_H */
