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
    F(AstArray), \
    F(AstDec), \
    F(AstIf), \
    F(AstFn), \
    F(AstFnCall), \
    F(AstLoop), \
    F(AstContinue), \
    F(AstBreak), \
    F(AstStruct), \
    F(AstImpl), \
    F(AstAttribute), \
    F(AstAffix), \
    F(AstUnaryExpr), \
    F(AstBinaryExpr), \
    F(AstIndex), \
    F(AstType), \
    F(AstSymbol), \
    F(AstReturn), \
    F(AstExtern), \

enum class AstNodeType {
    AstNodeTypes(AstNodeType_ENUM)
};
static const char *astNodeTypeNames[] = {
    AstNodeTypes(AstNodeType_NAME_ARRAY)
};

enum class AffixType {
    Infix,
    Prefix,
    Suffix,
};

struct AstNode {
    int line, column;
    AstNodeType type;

    AstNode(AstNodeType type): type(type) {}
    virtual ~AstNode() {}
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

struct AstArray: public AstNode {
    std::vector<std::unique_ptr<AstNode>> elements;

    AstArray(): AstNode(AstNodeType::AstArray) {}
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
    std::string name;
    bool is_array: 1;
    std::unique_ptr<AstType> subtype;

    AstType(): AstNode(AstNodeType::AstType) {}
};

struct AstDec: public AstNode {
    std::unique_ptr<AstSymbol> name;
    std::unique_ptr<AstType> type;
    std::unique_ptr<AstNode> value;
    bool immutable: 1;

    AstDec(): AstNode(AstNodeType::AstDec) {}
};

struct AstIf: public AstNode {
    std::unique_ptr<AstNode> condition;
    std::unique_ptr<AstBlock> true_block, false_block;

    AstIf(): AstNode(AstNodeType::AstIf) {}
};

struct AstFn: public AstNode {
    std::unique_ptr<AstSymbol> type_self;
    std::unique_ptr<AstSymbol> name;
    std::vector<std::unique_ptr<AstDec>> params;
    std::unique_ptr<AstType> return_type;
    std::unique_ptr<AstBlock> body;

    AstFn(): AstNode(AstNodeType::AstFn) {}
};

struct AstFnCall: public AstNode {
    std::unique_ptr<AstSymbol> name;
    std::vector<std::unique_ptr<AstNode>> args;

    AstFnCall(): AstNode(AstNodeType::AstFnCall) {}
};

struct AstLoop: public AstNode {
    bool is_foreach: 1;
    std::unique_ptr<AstBlock> body;
    std::unique_ptr<AstSymbol> name;
    std::unique_ptr<AstNode> expr;

    AstLoop(): AstNode(AstNodeType::AstLoop) {}
};

struct AstContinue: public AstNode {
    AstContinue(): AstNode(AstNodeType::AstContinue) {}
};

struct AstBreak: public AstNode {
    AstBreak(): AstNode(AstNodeType::AstBreak) {}
};

struct AstStruct: public AstNode {
    std::unique_ptr<AstSymbol> name;
    std::unique_ptr<AstBlock> block;

    AstStruct(): AstNode(AstNodeType::AstStruct) {}
};

struct AstImpl: public AstNode {
    std::unique_ptr<AstSymbol> name;
    std::unique_ptr<AstBlock> block;

    AstImpl(): AstNode(AstNodeType::AstImpl) {}
};

struct AstAttribute: public AstNode {
    std::unique_ptr<AstSymbol> name;
    std::vector<std::unique_ptr<AstNode>> args;

    AstAttribute(): AstNode(AstNodeType::AstAttribute) {}
};

struct AstAffix: public AstNode {
    std::unique_ptr<AstSymbol> name;
    std::vector<std::unique_ptr<AstDec>> params;
    std::unique_ptr<AstType> return_type;
    std::unique_ptr<AstBlock> body;
    AffixType affix_type;

    AstAffix(): AstNode(AstNodeType::AstAffix) {}
};

struct AstReturn: public AstNode {
    std::unique_ptr<AstNode> expr;

    AstReturn(): AstNode(AstNodeType::AstReturn) {}
};

struct AstUnaryExpr: public AstNode {
    std::string op;
    std::unique_ptr<AstNode> expr;

    AstUnaryExpr(): AstNode(AstNodeType::AstUnaryExpr) {}
};

struct AstBinaryExpr: public AstNode {
    std::string op;
    std::unique_ptr<AstNode> lhs, rhs;

    AstBinaryExpr(): AstNode(AstNodeType::AstBinaryExpr) {}
};

struct AstIndex: public AstNode {
    std::unique_ptr<AstNode> array, expr;

    AstIndex(): AstNode(AstNodeType::AstIndex) {}
};

struct AstExtern: public AstNode {
    std::vector<std::unique_ptr<AstFn>> decls;

    AstExtern(): AstNode(AstNodeType::AstExtern) {}
};

struct Ast {
    std::unique_ptr<AstBlock> root;
};

#endif /* AST_H */
