//
// Created by myvar on 5/6/18.
//

#include "Semantics.h"

void Semantics::p1_fn(const AstFn *node) {
    p1_funcs.push_back(node->name);
}

void Semantics::p1_struct(const AstStruct *node) {
    p1_structs.push_back(node->name);
}


void Semantics::pass1_node(const AstNode *node) {
    switch (node->node_type) {
        case AstNodeType::AstFn:
            p1_fn((const AstFn *) node);
            break;
        case AstNodeType::AstStruct:
            p1_struct((const AstStruct *) node);
            break;
        case AstNodeType::AstBlock:
            for (auto x: ((const AstBlock *) node)->statements) {
                pass1_node(x);
            }
            break;
        case AstNodeType::AstImpl:
            pass1_node(((const AstImpl *) node)->block);
            break;
        case AstNodeType::AstExtern:
            for (auto x: ((const AstExtern *) node)->decls) {
                pass1_node(x);
            }

            break;

    }
}

void Semantics::pass1(const Ast &ast) {
    pass1_node(ast.root);

    for(auto x: p1_structs)
    {
        printf(x->name.c_str());
    }
}


