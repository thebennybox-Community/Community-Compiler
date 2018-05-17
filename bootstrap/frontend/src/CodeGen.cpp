#include "ICodeGenerator.h"

class AstBlockCodeGenerator : public ICodeGenerator {
public:

    AstBlockCodeGenerator() {
        type_handler = AstNodeType::AstBlock;

    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary)  {

    }


};

class AstStringCodeGenerator : public ICodeGenerator {
public:

    AstStringCodeGenerator() {
        type_handler = AstNodeType::AstString;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstNumberCodeGenerator : public ICodeGenerator {
public:

    AstNumberCodeGenerator() {
        type_handler = AstNodeType::AstNumber;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstBooleanCodeGenerator : public ICodeGenerator {
public:

    AstBooleanCodeGenerator() {
        type_handler = AstNodeType::AstBoolean;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstArrayCodeGenerator : public ICodeGenerator {
public:

    AstArrayCodeGenerator() {
        type_handler = AstNodeType::AstArray;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstDecCodeGenerator : public ICodeGenerator {
public:

    AstDecCodeGenerator() {
        type_handler = AstNodeType::AstDec;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstIfCodeGenerator : public ICodeGenerator {
public:

    AstIfCodeGenerator() {
        type_handler = AstNodeType::AstIf;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstFnCodeGenerator : public ICodeGenerator {
public:

    AstFnCodeGenerator() {
        type_handler = AstNodeType::AstFn;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstFnCallCodeGenerator : public ICodeGenerator {
public:

    AstFnCallCodeGenerator() {
        type_handler = AstNodeType::AstFnCall;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstLoopCodeGenerator : public ICodeGenerator {
public:

    AstLoopCodeGenerator() {
        type_handler = AstNodeType::AstLoop;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstContinueCodeGenerator : public ICodeGenerator {
public:

    AstContinueCodeGenerator() {
        type_handler = AstNodeType::AstContinue;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstBreakCodeGenerator : public ICodeGenerator {
public:

    AstBreakCodeGenerator() {
        type_handler = AstNodeType::AstBreak;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstStructCodeGenerator : public ICodeGenerator {
public:

    AstStructCodeGenerator() {
        type_handler = AstNodeType::AstStruct;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstImplCodeGenerator : public ICodeGenerator {
public:

    AstImplCodeGenerator() {
        type_handler = AstNodeType::AstImpl;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstAttributeCodeGenerator : public ICodeGenerator {
public:

    AstAttributeCodeGenerator() {
        type_handler = AstNodeType::AstAttribute;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstAffixCodeGenerator : public ICodeGenerator {
public:

    AstAffixCodeGenerator() {
        type_handler = AstNodeType::AstAffix;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstUnaryExprCodeGenerator : public ICodeGenerator {
public:

    AstUnaryExprCodeGenerator() {
        type_handler = AstNodeType::AstUnaryExpr;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstBinaryExprCodeGenerator : public ICodeGenerator {
public:

    AstBinaryExprCodeGenerator() {
        type_handler = AstNodeType::AstBinaryExpr;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstIndexCodeGenerator : public ICodeGenerator {
public:

    AstIndexCodeGenerator() {
        type_handler = AstNodeType::AstIndex;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstTypeCodeGenerator : public ICodeGenerator {
public:

    AstTypeCodeGenerator() {
        type_handler = AstNodeType::AstType;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstSymbolCodeGenerator : public ICodeGenerator {
public:

    AstSymbolCodeGenerator() {
        type_handler = AstNodeType::AstSymbol;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstReturnCodeGenerator : public ICodeGenerator {
public:

    AstReturnCodeGenerator() {
        type_handler = AstNodeType::AstReturn;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstExternCodeGenerator : public ICodeGenerator {
public:

    AstExternCodeGenerator() {
        type_handler = AstNodeType::AstExtern;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstUseCodeGenerator : public ICodeGenerator {
public:

    AstUseCodeGenerator() {
        type_handler = AstNodeType::AstUse;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};

class AstNamespaceCodeGenerator : public ICodeGenerator {
public:

    AstNamespaceCodeGenerator() {
        type_handler = AstNodeType::AstNamespace;


    }

    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILemitter &binary) {

    }


};
