#ifndef ASTDEFS_H
#define ASTDEFS_H

#include <string>
#include "Token.h"

#define AstNodeType_ENUM(name) name
#define AstNodeType_NAME_ARRAY(name) #name

#define AstNodeTypes(F) \
    F(AstBlock),        \
    F(AstString),       \
    F(AstNumber),       \
    F(AstBoolean),      \
    F(AstArray),        \
    F(AstDec),          \
    F(AstIf),           \
    F(AstFn),           \
    F(AstFnCall),       \
    F(AstLoop),         \
    F(AstContinue),     \
    F(AstBreak),        \
    F(AstStruct),       \
    F(AstImpl),         \
    F(AstAttribute),    \
    F(AstAffix),        \
    F(AstUnaryExpr),    \
    F(AstBinaryExpr),   \
    F(AstIndex),        \
    F(AstType),         \
    F(AstSymbol),       \
    F(AstReturn),       \
    F(AstExtern),       \
    F(AstUse),          \
    F(AstNamespace),    \

enum class AstNodeType {
    AstNodeTypes(AstNodeType_ENUM)
};

static constexpr const char *const ast_node_type_names[] = {
    AstNodeTypes(AstNodeType_NAME_ARRAY)
};

enum class AffixType {
    Infix,
    Prefix,
    Suffix,
};

typedef struct AstNode AstNode;
typedef struct AstBlock AstBlock;
typedef struct AstString AstString;
typedef struct AstNumber AstNumber;
typedef struct AstBoolean AstBoolean;
typedef struct AstArray AstArray;
typedef struct AstDec AstDec;
typedef struct AstIf AstIf;
typedef struct AstFn AstFn;
typedef struct AstFnCall AstFnCall;
typedef struct AstLoop AstLoop;
typedef struct AstContinue AstContinue;
typedef struct AstBreak AstBreak;
typedef struct AstStruct AstStruct;
typedef struct AstImpl AstImpl;
typedef struct AstAttribute AstAttribute;
typedef struct AstAffix AstAffix;
typedef struct AstUnaryExpr AstUnaryExpr;
typedef struct AstBinaryExpr AstBinaryExpr;
typedef struct AstIndex AstIndex;
typedef struct AstType AstType;
typedef struct AstSymbol AstSymbol;
typedef struct AstReturn AstReturn;
typedef struct AstExtern AstExtern;
typedef struct AstUse AstUse;
typedef struct AstNamespace AstNamespace;

struct Ast {
    AstBlock *root = nullptr;
    std::vector<Token> tokens;
    std::string file_contents;
};

#endif /* ASTDEFS_H */
