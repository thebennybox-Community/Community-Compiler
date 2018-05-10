#ifndef FRONTEND_SEMANTICS_H
#define FRONTEND_SEMANTICS_H

#include <vector>
#include <string>
#include "AstDefs.h"

class Semantics {
public:
    void pass1(Ast &ast);
    void pass2(Ast &ast);
    void pass3(Ast &ast);

    bool nest_flag = false;
    std::vector<AstAttribute *> nested_attributes;

    std::vector<AstSymbol *> p1_funcs;
    std::vector<AstSymbol *> p1_structs;

    bool p1_hasSymbol(AstSymbol *z);
    bool p1_hasSymbol(AstType *y);
    AstFn *p2_get_fn(AstSymbol *name);
    AstFn *p2_get_fn(std::string &name);
    AstAffix *p2_get_affix(AstSymbol *name);
    AstAffix *p2_get_affix(std::string &name);
    AstDec *p2_get_dec(AstSymbol *name);
    AstDec *p2_get_dec(std::string &name);

    std::vector<AstFn *> p2_funcs;
    std::vector<AstAffix *> p2_affixs;
    std::vector<AstStruct *> p2_structs;
    std::vector<AstDec *> p2_dec;

    void pass1_node(AstNode *node);
    void p1_struct(AstStruct *node);
    void p1_fn(AstFn *node);

    void pass2_node(AstNode *node);
    void p2_struct(AstStruct *node);
    void p2_fn(AstFn *node);
    void p2_affix(AstAffix *node);

    void pass3_node(AstNode *node);
    void pass3_nest_att(AstNode *node);
    void p3_struct(AstStruct *node);
    void p3_affix(AstAffix *node);

    AstNode *inline_if_need_be(AstNode *node);
    AstType *determin_type(AstNode *node);
};

#endif // FRONTEND_SEMANTICS_H
