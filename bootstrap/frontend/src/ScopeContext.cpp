#include "ScopeContext.h"
#include "Ast.h"

void ScopeContext::import_scope(ScopeContext *scope) {

}

void ScopeContext::enter(AstNode *node) {
    auto sp = new Scope();
    sp->owner = node;
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
                              std::vector<AstType *> param_types) {
    for(auto func : funcs_global) {
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