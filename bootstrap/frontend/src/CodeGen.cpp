
#include "CodeGen.h"

void CodeGen::generateIL(AstNode *node) {
    switch(node->node_type) {
    case AstNodeType::AstBlock: {
        auto x = (AstBlock *)node;
        for(auto y : x->statements) { generateIL(y); }
        break;
    }
    case AstNodeType::AstString: {
        auto x = (AstString *)node;

        break;
    }
    case AstNodeType::AstNumber: {
        auto x = (AstNumber *)node;

        break;
    }
    case AstNodeType::AstBoolean: {
        auto x = (AstBoolean *)node;

        break;
    }
    case AstNodeType::AstArray: {
        auto x = (AstArray *)node;

        break;
    }
    case AstNodeType::AstDec: {
        auto x = (AstDec *)node;

        break;
    }
    case AstNodeType::AstIf: {
        auto x = (AstIf *)node;

        break;
    }
    case AstNodeType::AstFn: {
        auto x = (AstFn *)node;

        break;
    }
    case AstNodeType::AstFnCall: {
        auto x = (AstFnCall *)node;

        break;
    }
    case AstNodeType::AstLoop: {
        auto x = (AstLoop *)node;

        break;
    }
    case AstNodeType::AstContinue: {
        auto x = (AstContinue *)node;

        break;
    }
    case AstNodeType::AstBreak: {
        auto x = (AstBreak *)node;

        break;
    }
    case AstNodeType::AstStruct: {
        auto x = (AstStruct *)node;

        break;
    }
    case AstNodeType::AstImpl: {
        auto x = (AstImpl *)node;

        break;
    }
    case AstNodeType::AstAttribute: {
        auto x = (AstAttribute *)node;

        break;
    }
    case AstNodeType::AstAffix: {
        auto x = (AstAffix *)node;

        break;
    }
    case AstNodeType::AstUnaryExpr: {
        auto x = (AstUnaryExpr *)node;

        break;
    }
    case AstNodeType::AstBinaryExpr: {
        auto x = (AstBinaryExpr *)node;

        break;
    }
    case AstNodeType::AstIndex: {
        auto x = (AstIndex *)node;

        break;
    }
    case AstNodeType::AstType: {
        auto x = (AstType *)node;

        break;
    }
    case AstNodeType::AstSymbol: {
        auto x = (AstSymbol *)node;

        break;
    }
    case AstNodeType::AstReturn: {
        auto x = (AstReturn *)node;

        break;
    }
    case AstNodeType::AstExtern: {
        auto x = (AstExtern *)node;

        break;
    }
    }
}