#ifndef AST_H
#define AST_H

#include <vector>
#include <string>

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
static constexpr const char *const ast_node_type_names[] = {
    AstNodeTypes(AstNodeType_NAME_ARRAY)
};

enum class AffixType {
    Infix,
    Prefix,
    Suffix,
};

typedef struct AstAttribute AstAttribute;

struct AstNode {
    unsigned int line, column;
    AstNodeType node_type;

    std::vector<AstAttribute*> nested_attributes;

    AstNode(AstNodeType node_type): node_type(node_type) {}
    virtual ~AstNode() {}
};

struct AstString: public AstNode {
    std::string value;

    AstString(): AstNode(AstNodeType::AstString) {}
};

struct AstNumber: public AstNode {
    bool is_float  = false;
    bool is_signed = true;
    int bits;

    union {
        uint64_t u;
        int64_t  i;
        double   f;
    } value;

    AstNumber(): AstNode(AstNodeType::AstNumber) {}
};

struct AstBoolean: public AstNode {
    bool value;

    AstBoolean(): AstNode(AstNodeType::AstBoolean) {}
};

struct AstArray: public AstNode {
    std::vector<AstNode*> elements;

    AstArray(): AstNode(AstNodeType::AstArray) {}
    virtual ~AstArray() {
        for(auto p : elements) { delete p; }
    }
};

struct AstSymbol: public AstNode {
    std::string name;

    AstSymbol(): AstNode(AstNodeType::AstSymbol) {}
};

struct AstBlock: public AstNode {
    std::vector<AstNode*> statements;

    AstBlock(): AstNode(AstNodeType::AstBlock) {}
    virtual ~AstBlock() {
        for(auto *p : statements) { delete p; }
    }
};

struct AstType: public AstNode {
    std::string name;
    bool is_array = false;
    AstType *subtype  = nullptr;

    AstType(): AstNode(AstNodeType::AstType) {}
    virtual ~AstType() {
        if(subtype) delete subtype;
    }
};

struct AstDec: public AstNode {
    AstSymbol *name  = nullptr;
    AstType   *type  = nullptr;
    AstNode   *value = nullptr;
    bool immutable = false;

    AstDec(): AstNode(AstNodeType::AstDec) {}
    virtual ~AstDec() {
        if(name) delete name;
        if(type) delete type;
        if(value) delete value;
    }
};

struct AstIf: public AstNode {
    AstNode  *condition  = nullptr;
    AstBlock *true_block = nullptr, *false_block = nullptr;

    AstIf(): AstNode(AstNodeType::AstIf) {}
    virtual ~AstIf() {
        if(condition) delete condition;
        if(true_block) delete true_block;
        if(false_block) delete false_block;
    }
};

struct AstFn: public AstNode {
    AstSymbol *type_self = nullptr;
    AstSymbol *name      = nullptr;
    std::vector<AstDec*> params;
    AstType *return_type = nullptr;
    AstBlock *body       = nullptr;

    AstFn(): AstNode(AstNodeType::AstFn) {}
    virtual ~AstFn() {
        if(type_self) delete type_self;
        if(name) delete name;
        for(auto *p : params) { delete p; }
        if(return_type) delete return_type;
        if(body) delete body;
    }
};

struct AstFnCall: public AstNode {
    AstSymbol *name = nullptr;
    std::vector<AstNode *> args;

    AstFnCall(): AstNode(AstNodeType::AstFnCall) {}
    virtual ~AstFnCall() {
        if(name) delete name;
        for(auto *p : args) { delete p; }
    }
};

struct AstLoop: public AstNode {
    bool is_foreach = false;
    AstBlock  *body = nullptr;
    AstSymbol *name = nullptr;
    AstNode   *expr = nullptr;

    AstLoop(): AstNode(AstNodeType::AstLoop) {}
    virtual ~AstLoop() {
        if(body) delete body;
        if(name) delete name;
        if(expr) delete expr;
    }
};

struct AstContinue: public AstNode {
    AstContinue(): AstNode(AstNodeType::AstContinue) {}
};

struct AstBreak: public AstNode {
    AstBreak(): AstNode(AstNodeType::AstBreak) {}
};

struct AstStruct: public AstNode {
    AstSymbol *name  = nullptr;
    AstBlock  *block = nullptr;

    AstStruct(): AstNode(AstNodeType::AstStruct) {}
    virtual ~AstStruct() {
        if(name) delete name;
        if(block) delete block;
    }
};

struct AstImpl: public AstNode {
    AstSymbol *name  = nullptr;
    AstBlock  *block = nullptr;

    AstImpl(): AstNode(AstNodeType::AstImpl) {}
    virtual ~AstImpl() {
        if(name) delete name;
        if(block) delete block;
    }
};

struct AstAttribute: public AstNode {
    AstSymbol *name = nullptr;
    std::vector<AstNode*> args;

    AstAttribute(): AstNode(AstNodeType::AstAttribute) {}
    virtual ~AstAttribute() {
        if(name) delete name;
        for(auto *p : args) { delete p; }
    }
};

struct AstAffix: public AstNode {
    AstSymbol *name = nullptr;
    std::vector<AstDec *> params;
    AstType  *return_type = nullptr;
    AstBlock *body        = nullptr;
    AffixType affix_type;

    AstAffix(): AstNode(AstNodeType::AstAffix) {}
    virtual ~AstAffix() {
        if(name) delete name;
        for(auto *p : params) { delete p; }
        if(return_type) delete return_type;
        if(body) delete body;
    }
};

struct AstReturn: public AstNode {
    AstNode *expr = nullptr;

    AstReturn(): AstNode(AstNodeType::AstReturn) {}
    virtual ~AstReturn() {
        if(expr) delete expr;
    }
};

struct AstUnaryExpr: public AstNode {
    std::string op;
    AstNode *expr = nullptr;

    AstUnaryExpr(): AstNode(AstNodeType::AstUnaryExpr) {}
    virtual ~AstUnaryExpr() {
        if(expr) delete expr;
    }
};

struct AstBinaryExpr: public AstNode {
    std::string op;
    AstNode *lhs = nullptr, *rhs = nullptr;

    AstBinaryExpr(): AstNode(AstNodeType::AstBinaryExpr) {}
    virtual ~AstBinaryExpr() {
        if(lhs) delete lhs;
        if(rhs) delete rhs;
    }
};

struct AstIndex: public AstNode {
    AstNode *array = nullptr, *expr = nullptr;

    AstIndex(): AstNode(AstNodeType::AstIndex) {}
    virtual ~AstIndex() {
        if(array) delete array;
        if(expr) delete expr;
    }
};

struct AstExtern: public AstNode {
    std::vector<AstFn*> decls;

    AstExtern(): AstNode(AstNodeType::AstExtern) {}
    virtual ~AstExtern() {
        for(auto *p : decls) { delete p; }
    }
};

struct Ast {
    AstBlock *root = nullptr;

    ~Ast() {
        if(root) delete root;
    }
};

#endif /* AST_H */
