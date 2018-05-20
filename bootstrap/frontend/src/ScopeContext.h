#ifndef SCOPE_CONTEXT_H
#define SCOPE_CONTEXT_H

#include <vector>
#include <stack>
#include <stdint.h>
#include "AstDefs.h"
#include "Scope.h"

class ScopeContext {
public:
    AstNamespace *ns;

    void import_scope(ScopeContext *scope);

    void enter(AstNode *node, std::string name);
    void leave();

    bool func_exists(std::string name, std::vector<AstType *> param_types);
    AstFn *func_get(std::string name, std::vector<AstType *> param_types);
    AstFn *func_get(std::string name, std::vector<AstDec *> param_types);
    void func_add(AstFn *fn);

    bool affix_exists(std::string name, std::vector<AstType *> param_types);
    AstAffix *affix_get(std::string name, std::vector<AstType *> param_types);
    AstAffix *affix_get(std::string name, std::vector<AstDec *> param_types);
    void affix_add(AstAffix *affix);

    bool struct_exists(std::string name);
    AstStruct *struct_get(std::string name);
    void struct_add(AstStruct *struc);

    bool global_exists(std::string name);
    AstDec *global_get(std::string name);
    void global_add(AstDec *dec);

    bool arg_exists(std::string name);
    AstDec *arg_get(std::string name);
    void arg_add(AstDec *arg);

    bool local_exists(std::string name);
    AstDec *local_get(std::string name);
    void local_add(AstDec *local);

    std::vector<Scope *> scope_stack;

private:
    std::vector<AstFn *> funcs_global;
    std::vector<AstAffix *> affixes_global;
    std::vector<AstDec *> decs_global;
    std::vector<AstStruct *> structs_global;

};

#endif /* SCOPE_CONTEXT_H */
