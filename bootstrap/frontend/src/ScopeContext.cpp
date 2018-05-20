#include "ScopeContext.h"
#include "Ast.h"

static AstType *clone_type(const AstType *type) {
    auto clone = new AstType();
    auto result = clone;
    clone->name = type->name;

    while(type->subtype) {
        clone->subtype = new AstType();
        clone->subtype->name = type->subtype->name;
        type = type->subtype;
        clone = clone->subtype;
    }

    return result;
}


void ScopeContext::import_scope(ScopeContext *scope) {

}

static int g_counter = 0;

void ScopeContext::enter(AstNode *node, std::string name) {
    auto sp = new Scope();
    sp->owner = node;
    sp->owner_name = name;
    sp->id = g_counter++;
    sp->entry_label = "enter_scope_" + std::to_string(sp->id);
    sp->exit_label = "exit_scope_" + std::to_string(sp->id);
    scope_stack.push_back(sp);
}
void ScopeContext::leave() {
    scope_stack.pop_back();
}

bool ScopeContext::func_exists(std::string name,
                               std::vector<AstType *> param_types) {
    for(auto func : funcs_global) {
        if(func->name == name) {
            int c = 0;

            for(auto param : func->params) {
                if(param->type->name != param_types[c++]->name) {
                    break;
                }
            }

            return true;
        }
    }

    return false;
}


AstFn *ScopeContext::func_get(std::string name,
                              const std::vector<AstDec *> &param_types) {
    for(auto func : funcs_global) {
        if(func->name == name) {
            int c = 0;

            if(func->params.size() == param_types.size()) {
                for(auto param : func->params) {
                    if(param->type->name != param_types[c++]->type->name) {
                        break;
                    }
                }
            }

            return func;
        }
    }

    return nullptr;

}


AstFn *ScopeContext::func_get(std::string name,
                              const std::vector<AstType *> &param_types) {
    for(auto func : funcs_global) {
        if(func->name == name) {
            int c = 0;

            if(func->params.size() == param_types.size()) {
                for(auto param : func->params) {
                    if(param->type->name != param_types[c++]->name) {
                        break;
                    }
                }
            }

            return func;
        }
    }

    return nullptr;

}

void ScopeContext::func_add(AstFn *fn) {
    funcs_global.push_back(fn);
}

bool ScopeContext::affix_exists(std::string name,
                                std::vector<AstType *> param_types) {
    for(auto func : affixes_global) {
        if(func->name == name) {
            int c = 0;

            for(auto param : func->params) {
                if(param->type->name != param_types[c++]->name) {
                    break;
                }
            }

            return true;
        }
    }

    return false;
}

AstAffix *ScopeContext::affix_get(std::string name,
                                  std::vector<AstType *> param_types) {
    for(auto func : affixes_global) {
        if(func->name == name) {
            int c = 0;

            for(auto param : func->params) {
                if(param->type->name != param_types[c++]->name) {
                    break;
                }
            }

            return func;
        }
    }

    return nullptr;
}

AstAffix *ScopeContext::affix_get(std::string name,
                                  std::vector<AstDec *> param_types) {
    for(auto func : affixes_global) {
        if(func->name == name) {
            int c = 0;

            for(auto param : func->params) {
                if(param->type->name != param_types[c++]->type->name) {
                    break;
                }
            }

            return func;
        }
    }

    return nullptr;
}
void ScopeContext::affix_add(AstAffix *affix) {
    affixes_global.push_back(affix);
}

bool ScopeContext::struct_exists(std::string name) {
    for(auto strct : structs_global) {
        if(strct->name == name) {
            return true;
        }
    }

    return false;
}

AstStruct *ScopeContext::struct_get(std::string name) {
    for(auto strct : structs_global) {
        if(strct->name == name) {
            return strct;
        }
    }

    return nullptr;
}

void ScopeContext::struct_add(AstStruct *struc) {
    structs_global.push_back(struc);
}

bool ScopeContext::global_exists(std::string name) {
    for(auto dec : decs_global) {
        if(dec->name == name) {
            return true;
        }
    }

    return false;
}

AstDec *ScopeContext::global_get(std::string name) {
    for(auto dec : decs_global) {
        if(dec->name == name) {
            return dec;
        }
    }

    return nullptr;
}

void ScopeContext::global_add(AstDec *dec) {
    decs_global.push_back(dec);
}

bool ScopeContext::arg_exists(std::string name) {
    for(auto scope : scope_stack) {
        for(AstDec *arg : scope->args) {
            if(arg->name == name) {
                return true;
            }
        }
    }

    return false;
}

AstDec *ScopeContext::arg_get(std::string name) {
    for(auto scope : scope_stack) {
        for(AstDec *arg : scope->args) {
            if(arg->name == name) {
                return arg;
            }
        }
    }

    return nullptr;
}

void ScopeContext::arg_add(AstDec *arg) {
    scope_stack.front()->args.push_back(arg);
}

bool ScopeContext::local_exists(std::string name) {
    for(auto scope : scope_stack) {
        for(AstDec *local : scope->locals) {
            if(local->name == name) {
                return true;
            }
        }
    }

    return false;
}

AstDec *ScopeContext::local_get(std::string name) {
    for(auto scope : scope_stack) {
        for(AstDec *local : scope->locals) {
            if(local->name == name) {
                return local;
            }
        }
    }

    return nullptr;
}

void ScopeContext::local_add(AstDec *local) {
    scope_stack.front()->locals.push_back(local);
}


AstType *ScopeContext::infer_type(AstNode *node) {

    if(!node) {
        return nullptr;
    }

    switch(node->node_type) {

    case AstNodeType::AstBlock: {
        auto x = (AstBlock *)node;

        return infer_type(x->statements.front());

        break;
    }

    case AstNodeType::AstString: {
        auto re = new AstType();
        re->name = "str";
        return re;

        break;
    }

    case AstNodeType::AstNumber: {
        auto number = (AstNumber *)node;
        auto ret = new AstType();

        if(number->is_float) {
            ret->name = "f" + std::to_string(number->bits);
        } else if(number->is_signed) {
            ret->name = "i" + std::to_string(number->bits);
        } else {
            ret->name = "u" + std::to_string(number->bits);
        }

        return ret;
    }

    case AstNodeType::AstBoolean: {
        auto ret = new AstType();
        ret->name = "bool";
        return ret;
    }

    case AstNodeType::AstArray: {
        auto x = (AstArray *)node;

        if(x->ele_type) {
            return x->ele_type;
        } else {
            auto tp = infer_type(x->elements[0]);
            auto re = new AstType();
            re->subtype = new AstType();
            re->subtype->name = tp->name;
            re->is_array = true;
            return re;
        }

        break;
    }

    case AstNodeType::AstDec: {
        auto decl = (AstDec *)node;
        local_add(decl);
        return clone_type(decl->type);
        break;
    }

    case AstNodeType::AstFn: {
        return clone_type(((AstFn *)node)->return_type);
        break;
    }

    case AstNodeType::AstFnCall: {
        auto x = (AstFnCall *)node;
        auto types = std::vector<AstType *>();

        for(auto arg : x->args) {
            types.push_back(infer_type(arg));
        }

        auto fn = func_get(x->name, types);
        return clone_type(fn->return_type);

        break;
    }


    case AstNodeType::AstStruct: {
        auto ret = new AstType();
        ret->name = ((AstStruct *)node)->name;
        return ret;
        break;
    }

    case AstNodeType::AstAffix: {
        return clone_type(((AstAffix *)node)->return_type);
    }

    case AstNodeType::AstUnaryExpr: {
        break;
    }

    case AstNodeType::AstBinaryExpr: {
        auto bin_expr = (AstBinaryExpr *)node;

        auto types = std::vector<AstType *>();
        types.push_back(infer_type(bin_expr->lhs));
        types.push_back(infer_type(bin_expr->rhs));

        auto affix = affix_get(bin_expr->op, types);

        if(affix) {
            return clone_type(affix->return_type);
        }

        break;
    }

    case AstNodeType::AstIndex:
        return infer_type(((AstIndex *)node)->expr);

    case AstNodeType::AstType:
        return clone_type((AstType *)node);

    case AstNodeType::AstSymbol: {
        auto x = (AstSymbol *)node;
        {
            auto z = struct_get(x->name);

            if(z) {
                return infer_type(z);
            }
        }
        {
            auto z = global_get(x->name);

            if(z) {
                return z->type;
            }
        }
        {
            auto z = arg_get(x->name);

            if(z) {
                return z->type;
            }
        }
        {
            auto z = local_get(x->name);

            if(z) {
                return z->type;
            }
        }
        break;
    }
    }

    return nullptr;
}
