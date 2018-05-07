//
// Created by myvar on 5/6/18.
//

#ifndef FRONTEND_SEMANTICS_H
#define FRONTEND_SEMANTICS_H

#include "Ast.h"

class Semantics {
  public:
    void pass1(Ast &ast);
    void pass2(Ast &ast);
    void pass3(Ast &ast);

  private:
    bool                        nest_flag = false;
    std::vector<AstAttribute *> nested_attributes;

    std::vector<AstSymbol *> p1_funcs;
    std::vector<AstSymbol *> p1_structs;

    bool p1_hasSymbol(AstSymbol *z) {
        for(auto x : p1_funcs) {
            if(x->name == z->name) return true;
        }

        for(auto x : p1_structs) {
            if(x->name == z->name) return true;
        }

        return false;
    }

    bool p1_hasSymbol(AstType *y) {
        if(y == nullptr) return false;

        for(auto x : p1_funcs) {
            if(x->name == y->name) return true;
            if(p1_hasSymbol(y->subtype)) return true;
        }

        for(auto x : p1_structs) {
            if(x->name == y->name) return true;
            if(p1_hasSymbol(y->subtype)) return true;
        }

        return false;
    }

    std::vector<AstFn *>     p2_funcs;
    std::vector<AstStruct *> p2_structs;

    void pass1_node(AstNode *node);
    void p1_struct(AstStruct *node);
    void p1_fn(AstFn *node);

    void pass2_node(AstNode *node);
    void p2_struct(AstStruct *node);
    void p2_fn(AstFn *node);

    void pass3_node(AstNode *node);
    void pass3_nest_att(AstNode *node);
    void p3_struct(AstStruct *node);
    void p3_fn(AstFn *node);
    void p3_affix(AstAffix *node);
};

#endif // FRONTEND_SEMANTICS_H
