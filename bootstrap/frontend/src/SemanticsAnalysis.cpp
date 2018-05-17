#include "ISemanticGenerator.h"
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

        if(pass == 0) {
            scope->func_add((AstFn *)node);
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
