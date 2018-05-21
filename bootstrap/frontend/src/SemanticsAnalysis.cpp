#include "Ast.h"
#include "ISemanticAnalysis.h"



class AstBlockAnalysis : public ISemanticAnalysis {
public:

    AstBlockAnalysis() {
        type_handler = AstNodeType::AstBlock;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstStringAnalysis : public ISemanticAnalysis {
public:

    AstStringAnalysis() {
        type_handler = AstNodeType::AstString;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstNumberAnalysis : public ISemanticAnalysis {
public:

    AstNumberAnalysis() {
        type_handler = AstNodeType::AstNumber;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstBooleanAnalysis : public ISemanticAnalysis {
public:

    AstBooleanAnalysis() {
        type_handler = AstNodeType::AstBoolean;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstArrayAnalysis : public ISemanticAnalysis {
public:

    AstArrayAnalysis() {
        type_handler = AstNodeType::AstArray;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstDecAnalysis : public ISemanticAnalysis {
public:

    AstDecAnalysis() {
        type_handler = AstNodeType::AstDec;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        auto x = (AstDec *)node;

        if(!x->type) {
            x->type = scope->infer_type(x->value);
        }

        auto type = scope->infer_type(x->value);

        if(x->type->name != type->name) {
            printf("You can not assign an '%s' to and '%s'\n",
                   type->name.c_str(), x->type->name.c_str());
        }

        if(pass == 0) {
            scope->local_add(x);
        }

        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstIfAnalysis : public ISemanticAnalysis {
public:

    AstIfAnalysis() {
        type_handler = AstNodeType::AstIf;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstFnAnalysis : public ISemanticAnalysis {
public:

    AstFnAnalysis() {
        type_handler = AstNodeType::AstFn;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {

        auto x = (AstFn *)node;

        if(pass == 0) {
            scope->func_add(x);

            for(auto param : x->params) {
                scope->arg_add(param);
            }

            return true;
        }



        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstFnCallAnalysis : public ISemanticAnalysis {
public:

    AstFnCallAnalysis() {
        type_handler = AstNodeType::AstFnCall;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {

        auto x = (AstFnCall *)node;
        auto types = std::vector<AstType *>();

        for(auto arg : x->args) {
            types.push_back(scope->infer_type(arg));
        }

        auto fn = scope->func_get(x->name, types);

        if(x->args.size() == fn->params.size()) {

            for(int i = 0; i < fn->params.size(); i++) {
                auto a = fn->params[i];
                auto b = scope->infer_type(x->args[i]);

                if(a->type->name != b->name) {
                    printf("Invalid Argument provided in invoke of '%s' at offset '%d' of type '%s'"
                           "expected type '%s'\n",
                           x->name.c_str(), i + 1, b->name.c_str(), a->type->name.c_str());
                }
            }
        } else {
            printf("Invalid Amount of arguments provided\n");
        }

        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstLoopAnalysis : public ISemanticAnalysis {
public:

    AstLoopAnalysis() {
        type_handler = AstNodeType::AstLoop;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstContinueAnalysis : public ISemanticAnalysis {
public:

    AstContinueAnalysis() {
        type_handler = AstNodeType::AstContinue;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstBreakAnalysis : public ISemanticAnalysis {
public:

    AstBreakAnalysis() {
        type_handler = AstNodeType::AstBreak;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstStructAnalysis : public ISemanticAnalysis {
public:

    AstStructAnalysis() {
        type_handler = AstNodeType::AstStruct;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        if(pass == 0) {
            scope->struct_add((AstStruct *)node);
            return true;
        }

        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstImplAnalysis : public ISemanticAnalysis {
public:

    AstImplAnalysis() {
        type_handler = AstNodeType::AstImpl;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstAttributeAnalysis : public ISemanticAnalysis {
public:

    AstAttributeAnalysis() {
        type_handler = AstNodeType::AstAttribute;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstAffixAnalysis : public ISemanticAnalysis {
public:

    AstAffixAnalysis() {
        type_handler = AstNodeType::AstAffix;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        if(pass == 0) {
            scope->affix_add((AstAffix *)node);
            return true;
        }

        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstUnaryExprAnalysis : public ISemanticAnalysis {
public:

    AstUnaryExprAnalysis() {
        type_handler = AstNodeType::AstUnaryExpr;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstBinaryExprAnalysis : public ISemanticAnalysis {
public:

    AstBinaryExprAnalysis() {
        type_handler = AstNodeType::AstBinaryExpr;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstIndexAnalysis : public ISemanticAnalysis {
public:

    AstIndexAnalysis() {
        type_handler = AstNodeType::AstIndex;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstTypeAnalysis : public ISemanticAnalysis {
public:

    AstTypeAnalysis() {
        type_handler = AstNodeType::AstType;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstSymbolAnalysis : public ISemanticAnalysis {
public:

    AstSymbolAnalysis() {
        type_handler = AstNodeType::AstSymbol;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstReturnAnalysis : public ISemanticAnalysis {
public:

    AstReturnAnalysis() {
        type_handler = AstNodeType::AstReturn;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstExternAnalysis : public ISemanticAnalysis {
public:

    AstExternAnalysis() {
        type_handler = AstNodeType::AstExtern;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstUseAnalysis : public ISemanticAnalysis {
public:

    AstUseAnalysis() {
        type_handler = AstNodeType::AstUse;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};

class AstNamespaceAnalysis : public ISemanticAnalysis {
public:

    AstNamespaceAnalysis() {
        type_handler = AstNodeType::AstNamespace;
        pass = 0;

    }

    virtual bool validate_semantics(DuskAssembly &ds, ScopeContext *scope,
                                    AstNode *node) {
        return false;
    }

    virtual bool validate_types(DuskAssembly &ds, ScopeContext *scope,
                                AstNode *node) {
        return false;
    }
};
