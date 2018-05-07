//
// Created by myvar on 5/6/18.
//

#include "Ast.h"
#include "Semantics.h"

void Semantics::p1_fn(AstFn * node) {
    p1_funcs.push_back(node->name);
}

void Semantics::p1_struct(AstStruct * node) {
    p1_structs.push_back(node->name);
}


void Semantics::pass1_node(AstNode *node) {
    switch (node->node_type) {
        case AstNodeType::AstFn:
            p1_fn((AstFn *) node);
            break;
        case AstNodeType::AstStruct:
            p1_struct((AstStruct *) node);
            break;
        case AstNodeType::AstBlock:
            for (auto x: ((AstBlock *) node)->statements) {
                pass1_node(x);
            }
            break;
        case AstNodeType::AstImpl: {
            for (auto s:((AstImpl *) node)->block->statements) {
                if (s->node_type == AstNodeType::AstFn) {
                    AstSymbol * p = new AstSymbol();
                    p->name = ((AstImpl *) node)->name->name;
                    ((AstFn *) s)->type_self = p;
                }
            }

            pass1_node(((AstImpl *) node)->block);
        }
            break;
        case AstNodeType::AstExtern: {
            for (auto x: ((AstExtern *) node)->decls) {
                pass1_node(x);
            }
        }
            break;

    }
}

void Semantics::pass1(Ast &ast) {
    pass1_node(ast.root);

    for (auto x: p1_funcs) {
        //printf("%s\n", x->name.c_str());
    }
}

void Semantics::pass2(Ast &ast) {
    pass2_node(ast.root);

    for (auto x: p2_funcs) {
        if (x->type_self != nullptr) {
            printf("%s.%s ", x->type_self->name.c_str(), x->name->name.c_str());
        } else {
            printf("%s ", x->name->name.c_str());
        }

        for (auto a: x->params) {
            printf("%s: %s, ", a->name->name.c_str(), a->type->name.c_str());
        }
        printf("\n");
    }
}


void Semantics::pass2_node(AstNode *node) {
    switch (node->node_type) {
        case AstNodeType::AstFn:
            p2_fn((AstFn *) node);
              break;
        case AstNodeType::AstStruct:
            p2_struct((AstStruct *) node);
            break;
        case AstNodeType::AstBlock:
            for (auto x: ((AstBlock *) node)->statements) {
                pass2_node(x);
            }
            break;
        case AstNodeType::AstImpl: {
            pass2_node(((AstImpl *) node)->block);
        }
            break;
        case AstNodeType::AstExtern: {
            for (auto x: ((AstExtern *) node)->decls) {
                pass2_node(x);
            }
        }
            break;

    }
}

void Semantics::p2_fn(AstFn * node) {
    if (node->type_self != nullptr) {
        if (!p1_hasSymbol(node->type_self)) {
            printf("The type \"%s\" does not exist\n", node->type_self->name.c_str());
            return;
        }

        //we need to add implicit self
        AstDec * self = new AstDec();
        self->name = new AstSymbol();
        self->name->name = "self";

        self->type = new AstType();
        self->type->name = node->type_self->name;

        node->params.insert(node->params.begin(), self);

    }

    if (node->return_type != nullptr) {
        if (!p1_hasSymbol(node->return_type)) {
            printf("The type \"%s\" does not exist\n", node->return_type->name.c_str());
            return;
        }

    }


    for (auto a: node->params) {
        if (a->type != nullptr) {
            if (!p1_hasSymbol(a->type)) {
                printf("The type \"%s\" does not exist\n", a->type->name.c_str());
                return;
            }

        }
    }

    p2_funcs.push_back(node);

}

void Semantics::p2_struct(AstStruct * node) {

    for (auto b: node->block->statements) {
        if (b->node_type == AstNodeType::AstDec) {
            if (((AstDec *) b)->type != nullptr) {
                if (!p1_hasSymbol(((AstDec *) b)->type)) {
                    printf("The type \"%s\" does not exist\n", ((AstDec *) b)->type->name.c_str());
                    return;
                }
            }
        }
    }

}


void Semantics::pass3(Ast &ast) {

}
