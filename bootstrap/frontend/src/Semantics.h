#ifndef FRONTEND_SEMANTICS_H
#define FRONTEND_SEMANTICS_H

#include <vector>
#include <string>
#include "AstDefs.h"
#include "Error.h"

class Semantics {
public:
    void pass1(Ast &ast);
    void pass2(Ast &ast);
    void pass3(Ast &ast);

    bool p1_has_symbol(const std::string &symbol);
    bool p1_has_symbol(const AstType *type);
    AstFn *p2_get_fn(const AstSymbol *name);
    AstFn *p2_get_fn(const std::string &name);
    AstFn *p2_get_fn_unmangled(const std::string &name);
    AstFn *p2_get_fn_unmangled(const AstSymbol *name);
    AstAffix *p2_get_affix(const AstSymbol *name);
    AstAffix *p2_get_affix(const std::string &name);
    AstDec *p2_get_dec(const AstSymbol *name);
    AstDec *p2_get_dec(const std::string &name);

    AstType *infer_type(AstNode *node);

    std::vector<Error> errors;

private:
    std::vector<AstFn*> p2_funcs;
    std::vector<AstAffix*> p2_affixes;
    std::vector<AstStruct*> p2_structs;
    std::vector<AstDec*> p2_dec;

    bool nest_flag = false;
    std::vector<AstAttribute*> attributes;

    std::vector<std::string> p1_funcs;
    std::vector<std::string> p1_structs;

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
};

#endif // FRONTEND_SEMANTICS_H
