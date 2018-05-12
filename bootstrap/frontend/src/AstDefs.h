#ifndef ASTDEFS_H
#define ASTDEFS_H

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

struct Ast {
    AstBlock *root = nullptr;
};

#endif /* ASTDEFS_H */
