//
// Created by myvar on 5/6/18.
//

#ifndef FRONTEND_SEMANTICS_H
#define FRONTEND_SEMANTICS_H


#include "Ast.h"

class Semantics {

public:
    void pass1(const Ast &ast);

private:
    std::vector<AstSymbol*> p1_funcs;
    std::vector<AstSymbol*> p1_structs;


    void pass1_node(const AstNode *node);

    void p1_struct(const AstStruct *node);

    void p1_fn(const AstFn *node);
};


#endif //FRONTEND_SEMANTICS_H
