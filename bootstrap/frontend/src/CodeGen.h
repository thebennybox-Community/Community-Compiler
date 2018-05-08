#ifndef SRC_CODEGEN_H
#define SRC_CODEGEN_H

#include "Ast.h"
#include "ILemitter.h"
#include "Semantics.h"
#include <stack>
#include <vector>

class CodeGen {
public:
    void generateIL(AstNode *node, ILemitter &il);

    Semantics sem;

private:
    int g_counter;

    std::vector<AstDec *> scope;
    std::vector<AstDec *> args;
    std::stack<std::vector<AstDec *>> scope_stack;
    std::stack<std::vector<AstDec *>> arg_stack;

    AstSymbol *scope_owner;

    bool has_local(std::string name) {
        for(auto e : scope) {
            if(e->name->name == name) {
                return true;
            }
        }

        return false;
    }

    bool has_local(AstSymbol *name) {
        return has_local(name->name);
    }

    bool has_arg(std::string name) {
        for(auto e : args) {
            if(e->name->name == name) {
                return true;
            }
        }

        return false;
    }

    bool has_arg(AstSymbol *name) {
        return has_arg(name->name);
    }

    void add_local(AstDec *dec) {
        scope.push_back(dec);
    }

    void add_arg(AstDec *dec) {
        args.push_back(dec);
    }

    void push_scope() {
        scope_stack.push(scope);
        arg_stack.push(args);
    }

    void pop_scope() {
        scope = scope_stack.top();
        scope_stack.pop();

        args = arg_stack.top();
        arg_stack.pop();
    }
};

#endif // SRC_CODEGEN_H
