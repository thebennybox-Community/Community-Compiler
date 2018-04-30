#ifndef AST_H
#define AST_H

#include <memory>
#include <vector>

#define AstType_ENUM(name) name
#define AstType_NAME_ARRAY(name) #name

#define AstTypes(F) \
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

enum class AstType {
    AstTypes(AstType_ENUM)
};
static const char *tokenTypeNames[] = {
    AstTypes(AstType_NAME_ARRAY)
};

struct AstNode {
    int row, column;
    AstType type;
};

struct AstBlock: public AstNode {
    std::vector<std::unique_ptr<AstNode>> statements;

    AstBlock(): type(AstType::AstBlock) {}
};

struct AstString: public AstNode {
    std::string value;

    AstString(): type(AstType::AstString) {}
};

struct AstNumber: public AstNode {
    bool is_float: 1;
    bool is_signed: 1;

    union {
        uint64_t unsigned_value;
        int64_t signed_value;
        double float_value;
    };

    AstNumber(): type(AstType::AstNumber) {}
};

struct AstBoolean: public AstNode {
    bool value;

    AstBoolean(): type(AstType::AstBoolean) {}
};

struct AstDec: public AstNode {
    std::unique_ptr<AstSymbol> name;
    std::unique_ptr<AstType> type;
    std::unique_ptr<AstNode> value;

    AstDec(): type(AstType::AstDec) {}
};

struct AstFn: public AstNode {
    std::unique_ptr<AstSymbol> name;
    std::vector<std::unique_ptr<AstDec>> params;
    std::unique_ptr<AstType> return_type;
    std::unique_ptr<AstBlock> body;

    AstFn(): type(AstType::AstFn) {}
};

struct AstLoop: public AstNode {


    AstLoop(): type(AstType::AstLoop) {}
};

struct AstContinue: public AstNode {
    AstContinue(): type(AstType::AstContinue) {}
};

struct AstBreak: public AstNode {
    AstBreak(): type(AstType::AstBreak) {}
};

struct AstGet: public AstNode {


    AstGet(): type(AstType::AstGet) {}
};

struct AstSet: public AstNode {


    AstSet(): type(AstType::AstSet) {}
};

struct AstImpl: public AstNode {


    AstImpl(): type(AstType::AstImpl) {}
};

struct AstAttribute: public AstNode {


    AstAttribute(): type(AstType::AstAttribute) {}
};

struct AstOp: public AstNode {


    AstOp(): type(AstType::AstOp) {}
};

struct AstExpr: public AstNode {


    AstExpr(): type(AstType::AstExpr) {}
};

struct AstType: public AstNode {


    AstType(): type(AstType::AstType) {}
};

struct AstSymbol: public AstNode {
    std::string name;

    AstSymbol(): type(AstType::AstSymbol) {}
};

struct Ast {
    std::unique_ptr<AstBlock> root;
};

#endif /* AST_H */
