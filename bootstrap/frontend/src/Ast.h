#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include "Semantics.h"
#include "ILemitter.h"
#include "AstDefs.h"

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

struct AstNode {
    AstNodeType node_type;
    unsigned int line, column;
    bool emit = true;

    std::vector<AstAttribute*> attributes;

    AstNode(AstNodeType node_type, unsigned int line, unsigned int column):
        node_type(node_type), line(line), column(column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem) = 0;

    virtual ~AstNode() {}
};

struct AstString : public AstNode {
    std::string value;

    AstString(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstString, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);
};

struct AstNumber : public AstNode {
    bool is_float  = false;
    bool is_signed = true;
    int bits;

    union {
        uint64_t u;
        int64_t i;
        double f;
    } value;

    AstNumber(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstNumber, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);
};

struct AstBoolean : public AstNode {
    bool value;

    AstBoolean(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstBoolean, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);
};

struct AstArray : public AstNode {
    std::vector<AstNode*> elements;

    AstArray(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstArray, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstArray() {
        for(auto p : elements) {
            delete p;
        }
    }
};

struct AstSymbol : public AstNode {
    std::string name;

    AstSymbol(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstSymbol, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);
};

struct AstBlock : public AstNode {
    std::vector<AstNode*> statements;

    AstBlock(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstBlock, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstBlock() {
        for(auto *p : statements) {
            delete p;
        }
    }
};

struct AstType : public AstNode {
    std::string name;
    bool is_array    = false;
    AstType *subtype = nullptr;

    AstType(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstType, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstType() {
        delete subtype;
    }
};

struct AstDec : public AstNode {
    std::string name;
    AstType *type  = nullptr;
    AstNode *value = nullptr;
    bool immutable = false;

    AstDec(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstDec, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstDec() {
        delete type;
        delete value;
    }
};

struct AstIf : public AstNode {
    AstNode  *condition  = nullptr;
    AstBlock *true_block = nullptr, *false_block = nullptr;

    AstIf(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstIf, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstIf() {
        delete condition;
        delete true_block;
        delete false_block;
    }
};

struct AstFn : public AstNode {
    std::string unmangled_name;
    std::string mangled_name;
    std::string type_self;
    std::vector<AstDec*> params;
    AstType *return_type = nullptr;
    AstBlock *body       = nullptr;

    AstFn(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstFn, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstFn() {
        delete return_type;
        delete body;

        for(auto *p : params) {
            delete p;
        }
    }
};

struct AstFnCall : public AstNode {
    std::string name;
    std::vector<AstNode*> args;
    bool mangled = false;

    AstFnCall(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstFnCall, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstFnCall() {
        for(auto *p : args) {
            delete p;
        }
    }
};

struct AstLoop : public AstNode {
    std::string name;
    bool is_foreach = false;
    AstBlock *body  = nullptr;
    AstNode *expr   = nullptr;

    AstLoop(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstLoop, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstLoop() {
        delete body;
        delete expr;
    }
};

struct AstContinue: public AstNode {
    AstContinue(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstContinue, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);
};

struct AstBreak: public AstNode {
    AstBreak(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstBreak, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);
};

struct AstStruct : public AstNode {
    std::string name;
    AstBlock *block = nullptr;

    AstStruct(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstStruct, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstStruct() {
        delete block;
    }
};

struct AstImpl : public AstNode {
    std::string name;
    AstBlock *block = nullptr;

    AstImpl(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstImpl, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstImpl() {
        delete block;
    }
};

struct AstAttribute : public AstNode {
    std::string name;
    std::vector<AstNode*> args;

    AstAttribute(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstAttribute, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstAttribute() {
        for(auto *p : args) {
            delete p;
        }
    }
};

struct AstAffix : public AstNode {
    std::string name;
    std::vector<AstDec*> params;
    AstType *return_type = nullptr;
    AstBlock *body       = nullptr;
    AffixType affix_type;

    AstAffix(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstAffix, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstAffix() {
        delete return_type;
        delete body;

        for(auto *p : params) {
            delete p;
        }
    }
};

struct AstReturn : public AstNode {
    AstNode *expr = nullptr;

    AstReturn(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstReturn, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstReturn() {
        delete expr;
    }
};

struct AstUnaryExpr : public AstNode {
    std::string op;
    AstNode *expr = nullptr;

    AstUnaryExpr(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstUnaryExpr, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstUnaryExpr() {
        delete expr;
    }
};

struct AstBinaryExpr : public AstNode {
    std::string op;
    AstNode *lhs = nullptr, *rhs = nullptr;

    AstBinaryExpr(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstBinaryExpr, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstBinaryExpr() {
        delete lhs;
        delete rhs;
    }
};

struct AstIndex : public AstNode {
    AstNode *array = nullptr, *expr = nullptr;

    AstIndex(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstIndex, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstIndex() {
        delete array;
        delete expr;
    }
};

struct AstExtern : public AstNode {
    std::vector<AstFn*> decls;

    AstExtern(unsigned int line = 0, unsigned int column = 0):
        AstNode(AstNodeType::AstExtern, line, column) {}

    virtual void code_gen(ILemitter &il, Semantics &sem);

    virtual ~AstExtern() {
        for(auto *p : decls) {
            delete p;
        }
    }
};

#endif /* AST_H */
