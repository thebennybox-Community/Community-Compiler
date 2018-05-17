#include "ICodeGenerator.h"

class AstBlockGenerator : public ICodeGenerator {
public:

    AstBlockGenerator() {
        type_handler = AstNodeType::AstBlock;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstStringGenerator : public ICodeGenerator {
public:

    AstStringGenerator() {
        type_handler = AstNodeType::AstString;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstNumberGenerator : public ICodeGenerator {
public:

    AstNumberGenerator() {
        type_handler = AstNodeType::AstNumber;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstBooleanGenerator : public ICodeGenerator {
public:

    AstBooleanGenerator() {
        type_handler = AstNodeType::AstBoolean;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstArrayGenerator : public ICodeGenerator {
public:

    AstArrayGenerator() {
        type_handler = AstNodeType::AstArray;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstDecGenerator : public ICodeGenerator {
public:

    AstDecGenerator() {
        type_handler = AstNodeType::AstDec;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstIfGenerator : public ICodeGenerator {
public:

    AstIfGenerator() {
        type_handler = AstNodeType::AstIf;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstFnGenerator : public ICodeGenerator {
public:

    AstFnGenerator() {
        type_handler = AstNodeType::AstFn;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstFnCallGenerator : public ICodeGenerator {
public:

    AstFnCallGenerator() {
        type_handler = AstNodeType::AstFnCall;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstLoopGenerator : public ICodeGenerator {
public:

    AstLoopGenerator() {
        type_handler = AstNodeType::AstLoop;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstContinueGenerator : public ICodeGenerator {
public:

    AstContinueGenerator() {
        type_handler = AstNodeType::AstContinue;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstBreakGenerator : public ICodeGenerator {
public:

    AstBreakGenerator() {
        type_handler = AstNodeType::AstBreak;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstStructGenerator : public ICodeGenerator {
public:

    AstStructGenerator() {
        type_handler = AstNodeType::AstStruct;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstImplGenerator : public ICodeGenerator {
public:

    AstImplGenerator() {
        type_handler = AstNodeType::AstImpl;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstAttributeGenerator : public ICodeGenerator {
public:

    AstAttributeGenerator() {
        type_handler = AstNodeType::AstAttribute;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstAffixGenerator : public ICodeGenerator {
public:

    AstAffixGenerator() {
        type_handler = AstNodeType::AstAffix;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstUnaryExprGenerator : public ICodeGenerator {
public:

    AstUnaryExprGenerator() {
        type_handler = AstNodeType::AstUnaryExpr;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstBinaryExprGenerator : public ICodeGenerator {
public:

    AstBinaryExprGenerator() {
        type_handler = AstNodeType::AstBinaryExpr;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstIndexGenerator : public ICodeGenerator {
public:

    AstIndexGenerator() {
        type_handler = AstNodeType::AstIndex;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstTypeGenerator : public ICodeGenerator {
public:

    AstTypeGenerator() {
        type_handler = AstNodeType::AstType;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstSymbolGenerator : public ICodeGenerator {
public:

    AstSymbolGenerator() {
        type_handler = AstNodeType::AstSymbol;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstReturnGenerator : public ICodeGenerator {
public:

    AstReturnGenerator() {
        type_handler = AstNodeType::AstReturn;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstExternGenerator : public ICodeGenerator {
public:

    AstExternGenerator() {
        type_handler = AstNodeType::AstExtern;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstUseGenerator : public ICodeGenerator {
public:

    AstUseGenerator() {
        type_handler = AstNodeType::AstUse;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstNamespaceGenerator : public ICodeGenerator {
public:

    AstNamespaceGenerator() {
        type_handler = AstNodeType::AstNamespace;


    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};
