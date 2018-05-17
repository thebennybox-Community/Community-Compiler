#include "ICodeGenerator.h"

class AstBlockCodeGenerator : public ICodeGenerator {
public:

    AstBlockCodeGenerator() {
        type_handler = AstNodeType::AstBlock;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstStringCodeGenerator : public ICodeGenerator {
public:

    AstStringCodeGenerator() {
        type_handler = AstNodeType::AstString;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstNumberCodeGenerator : public ICodeGenerator {
public:

    AstNumberCodeGenerator() {
        type_handler = AstNodeType::AstNumber;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstBooleanCodeGenerator : public ICodeGenerator {
public:

    AstBooleanCodeGenerator() {
        type_handler = AstNodeType::AstBoolean;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstArrayCodeGenerator : public ICodeGenerator {
public:

    AstArrayCodeGenerator() {
        type_handler = AstNodeType::AstArray;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstDecCodeGenerator : public ICodeGenerator {
public:

    AstDecCodeGenerator() {
        type_handler = AstNodeType::AstDec;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstIfCodeGenerator : public ICodeGenerator {
public:

    AstIfCodeGenerator() {
        type_handler = AstNodeType::AstIf;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstFnCodeGenerator : public ICodeGenerator {
public:

    AstFnCodeGenerator() {
        type_handler = AstNodeType::AstFn;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstFnCallCodeGenerator : public ICodeGenerator {
public:

    AstFnCallCodeGenerator() {
        type_handler = AstNodeType::AstFnCall;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstLoopCodeGenerator : public ICodeGenerator {
public:

    AstLoopCodeGenerator() {
        type_handler = AstNodeType::AstLoop;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstContinueCodeGenerator : public ICodeGenerator {
public:

    AstContinueCodeGenerator() {
        type_handler = AstNodeType::AstContinue;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstBreakCodeGenerator : public ICodeGenerator {
public:

    AstBreakCodeGenerator() {
        type_handler = AstNodeType::AstBreak;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstStructCodeGenerator : public ICodeGenerator {
public:

    AstStructCodeGenerator() {
        type_handler = AstNodeType::AstStruct;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstImplCodeGenerator : public ICodeGenerator {
public:

    AstImplCodeGenerator() {
        type_handler = AstNodeType::AstImpl;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstAttributeCodeGenerator : public ICodeGenerator {
public:

    AstAttributeCodeGenerator() {
        type_handler = AstNodeType::AstAttribute;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstAffixCodeGenerator : public ICodeGenerator {
public:

    AstAffixCodeGenerator() {
        type_handler = AstNodeType::AstAffix;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstUnaryExprCodeGenerator : public ICodeGenerator {
public:

    AstUnaryExprCodeGenerator() {
        type_handler = AstNodeType::AstUnaryExpr;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstBinaryExprCodeGenerator : public ICodeGenerator {
public:

    AstBinaryExprCodeGenerator() {
        type_handler = AstNodeType::AstBinaryExpr;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstIndexCodeGenerator : public ICodeGenerator {
public:

    AstIndexCodeGenerator() {
        type_handler = AstNodeType::AstIndex;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstTypeCodeGenerator : public ICodeGenerator {
public:

    AstTypeCodeGenerator() {
        type_handler = AstNodeType::AstType;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstSymbolCodeGenerator : public ICodeGenerator {
public:

    AstSymbolCodeGenerator() {
        type_handler = AstNodeType::AstSymbol;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstReturnCodeGenerator : public ICodeGenerator {
public:

    AstReturnCodeGenerator() {
        type_handler = AstNodeType::AstReturn;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstExternCodeGenerator : public ICodeGenerator {
public:

    AstExternCodeGenerator() {
        type_handler = AstNodeType::AstExtern;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstUseCodeGenerator : public ICodeGenerator {
public:

    AstUseCodeGenerator() {
        type_handler = AstNodeType::AstUse;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstNamespaceCodeGenerator : public ICodeGenerator {
public:

    AstNamespaceCodeGenerator() {
        type_handler = AstNodeType::AstNamespace;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};
