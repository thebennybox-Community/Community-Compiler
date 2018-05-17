#include "ISemanticGenerator.h"

class AstBlockGenerator : public ISemanticGenerator {
public:

    AstBlockGenerator() {
        type_handler = AstNodeType::AstBlock;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstStringGenerator : public ISemanticGenerator {
public:

    AstStringGenerator() {
        type_handler = AstNodeType::AstString;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstNumberGenerator : public ISemanticGenerator {
public:

    AstNumberGenerator() {
        type_handler = AstNodeType::AstNumber;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstBooleanGenerator : public ISemanticGenerator {
public:

    AstBooleanGenerator() {
        type_handler = AstNodeType::AstBoolean;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstArrayGenerator : public ISemanticGenerator {
public:

    AstArrayGenerator() {
        type_handler = AstNodeType::AstArray;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstDecGenerator : public ISemanticGenerator {
public:

    AstDecGenerator() {
        type_handler = AstNodeType::AstDec;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstIfGenerator : public ISemanticGenerator {
public:

    AstIfGenerator() {
        type_handler = AstNodeType::AstIf;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstFnGenerator : public ISemanticGenerator {
public:

    AstFnGenerator() {
        type_handler = AstNodeType::AstFn;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstFnCallGenerator : public ISemanticGenerator {
public:

    AstFnCallGenerator() {
        type_handler = AstNodeType::AstFnCall;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstLoopGenerator : public ISemanticGenerator {
public:

    AstLoopGenerator() {
        type_handler = AstNodeType::AstLoop;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstContinueGenerator : public ISemanticGenerator {
public:

    AstContinueGenerator() {
        type_handler = AstNodeType::AstContinue;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstBreakGenerator : public ISemanticGenerator {
public:

    AstBreakGenerator() {
        type_handler = AstNodeType::AstBreak;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstStructGenerator : public ISemanticGenerator {
public:

    AstStructGenerator() {
        type_handler = AstNodeType::AstStruct;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstImplGenerator : public ISemanticGenerator {
public:

    AstImplGenerator() {
        type_handler = AstNodeType::AstImpl;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstAttributeGenerator : public ISemanticGenerator {
public:

    AstAttributeGenerator() {
        type_handler = AstNodeType::AstAttribute;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstAffixGenerator : public ISemanticGenerator {
public:

    AstAffixGenerator() {
        type_handler = AstNodeType::AstAffix;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstUnaryExprGenerator : public ISemanticGenerator {
public:

    AstUnaryExprGenerator() {
        type_handler = AstNodeType::AstUnaryExpr;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstBinaryExprGenerator : public ISemanticGenerator {
public:

    AstBinaryExprGenerator() {
        type_handler = AstNodeType::AstBinaryExpr;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstIndexGenerator : public ISemanticGenerator {
public:

    AstIndexGenerator() {
        type_handler = AstNodeType::AstIndex;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstTypeGenerator : public ISemanticGenerator {
public:

    AstTypeGenerator() {
        type_handler = AstNodeType::AstType;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstSymbolGenerator : public ISemanticGenerator {
public:

    AstSymbolGenerator() {
        type_handler = AstNodeType::AstSymbol;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstReturnGenerator : public ISemanticGenerator {
public:

    AstReturnGenerator() {
        type_handler = AstNodeType::AstReturn;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstExternGenerator : public ISemanticGenerator {
public:

    AstExternGenerator() {
        type_handler = AstNodeType::AstExtern;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstUseGenerator : public ISemanticGenerator {
public:

    AstUseGenerator() {
        type_handler = AstNodeType::AstUse;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};

class AstNamespaceGenerator : public ISemanticGenerator {
public:

    AstNamespaceGenerator() {
        type_handler = AstNodeType::AstNamespace;
        pass = 0;

    }

    virtual AstNode *generate(DuskAssembly &ds, ScopeContext *scope,
                              AstNode *node) {
        return nullptr;
    }


};
