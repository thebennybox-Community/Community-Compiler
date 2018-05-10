#ifndef SRC_CODEGEN_H
#define SRC_CODEGEN_H

#include "ILemitter.h"
#include "Semantics.h"
#include <stack>
#include <vector>

#include <algorithm>
#include "Ast.h"


// class CodeGen {
// public:
// static void generateIL(AstNode *node, ILemitter &il);



static AstSymbol *scope_owner;

static int g_counter;

static std::vector<AstDec *> scope;
static std::vector<AstDec *> args;
static std::stack<std::vector<AstDec *>> scope_stack;
static std::stack<std::vector<AstDec *>> arg_stack;




static bool has_local(std::string name) {
    for(auto e : scope) {
        if(e->name->name == name) {
            return true;
        }
    }

    return false;
}

static bool has_local(AstSymbol *name) {
    return has_local(name->name);
}

static bool has_arg(std::string name) {
    for(auto e : args) {
        if(e->name->name == name) {
            return true;
        }
    }

    return false;
}


static AstDec *get_local(std::string name) {
    for(auto e : scope) {
        if(e->name->name == name) {
            return e;
        }
    }

    return nullptr;
}

static AstDec *get_local(AstSymbol *name) {
    return get_local(name->name);
}



static bool has_arg(AstSymbol *name) {
    return has_arg(name->name);
}

static void add_local(AstDec *dec) {
    scope.push_back(dec);
}

static void add_arg(AstDec *dec) {
    args.push_back(dec);
}

static void  push_scope() {
    scope_stack.push(scope);
    arg_stack.push(args);
}

static void pop_scope() {
    scope = scope_stack.top();
    scope_stack.pop();

    args = arg_stack.top();
    arg_stack.pop();
}


static void generateIL(AstNode *node, ILemitter &il, Semantics &sem) {

    if(node == nullptr) {
        return;
    }

    if(!node->emit) {
        return;
    }



    node->code_gen(il, sem);
}


//};

#endif // SRC_CODEGEN_H
