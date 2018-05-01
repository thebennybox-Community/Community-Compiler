#ifndef AST_H
#define AST_H

#include <memory>
#include <vector>

#define AstNodeType_ENUM(name) name
#define AstNodeType_NAME_ARRAY(name) #name

#define AstNodeTypes(F) \
    F(AstBlock), \
    F(AstString), \
    F(AstNumber), \
    F(AstBoolean), \
    F(AstDec), \
    F(AstFn), \
    F(AstLoop), \
    F(AstContinue), \
    F(AstBreak), \
    F(AstGet), \
    F(AstSet), \
    F(AstImpl), \
    F(AstAttribute), \
    F(AstOp), \
    F(AstExpr), \
    F(AstType), \
    F(AstSymbol), \

enum class AstNodeType {
    AstNodeTypes(AstNodeType_ENUM)
};
static const char *astNodeTypeNames[] = {
    AstNodeTypes(AstNodeType_NAME_ARRAY)
};

struct AstNode {
    int line, column;
    AstNodeType type;

    AstNode(AstNodeType type): type(type) {}
};

struct AstString: public AstNode {
    std::string value;

    AstString(): AstNode(AstNodeType::AstString) {}
};

struct AstNumber: public AstNode {
    bool is_float: 1;
    bool is_signed: 1;
    int bits;

    union {
        uint64_t u;
        int64_t i;
        double f;
    } value;

    AstNumber(): AstNode(AstNodeType::AstNumber) {}
};

struct AstBoolean: public AstNode {
    bool value;

    AstBoolean(): AstNode(AstNodeType::AstBoolean) {}
};

struct AstSymbol: public AstNode {
    std::string name;

    AstSymbol(): AstNode(AstNodeType::AstSymbol) {}
};

struct AstBlock: public AstNode {
    std::vector<std::unique_ptr<AstNode>> statements;

    AstBlock(): AstNode(AstNodeType::AstBlock) {}
};

struct AstType: public AstNode {


    AstType(): AstNode(AstNodeType::AstType) {}
};

struct AstDec: public AstNode {
    std::unique_ptr<AstSymbol> name;
    std::unique_ptr<AstType> type;
    std::unique_ptr<AstNode> value;
    bool immutable: 1;

    AstDec(): AstNode(AstNodeType::AstDec) {}
};

struct AstFn: public AstNode {
    std::unique_ptr<AstSymbol> name;
    std::vector<std::unique_ptr<AstDec>> params;
    std::unique_ptr<AstType> return_type;
    std::unique_ptr<AstBlock> body;

    AstFn(): AstNode(AstNodeType::AstFn) {}
};

struct AstLoop: public AstNode {


    AstLoop(): AstNode(AstNodeType::AstLoop) {}
};

struct AstContinue: public AstNode {
    AstContinue(): AstNode(AstNodeType::AstContinue) {}
};

struct AstBreak: public AstNode {
    AstBreak(): AstNode(AstNodeType::AstBreak) {}
};

struct AstGet: public AstNode {


    AstGet(): AstNode(AstNodeType::AstGet) {}
};

struct AstSet: public AstNode {


    AstSet(): AstNode(AstNodeType::AstSet) {}
};

struct AstImpl: public AstNode {


    AstImpl(): AstNode(AstNodeType::AstImpl) {}
};

struct AstAttribute: public AstNode {


    AstAttribute(): AstNode(AstNodeType::AstAttribute) {}
};

struct AstOp: public AstNode {


    AstOp(): AstNode(AstNodeType::AstOp) {}
};

struct AstExpr: public AstNode {


    AstExpr(): AstNode(AstNodeType::AstExpr) {}
};

struct Ast {
    std::unique_ptr<AstBlock> root;
};

#endif /* AST_H */
