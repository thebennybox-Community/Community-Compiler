#include "ICodeGenerator.h"
#include "Ast.h"

static unsigned int g_counter = 0;

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
        auto x = (AstString *)node;
        binary.push_str(x->value.c_str());
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
        auto x = (AstNumber *)node;

        switch(x->bits) {
        case 8:
            if(x->is_float) {
                printf("Internal compiler error: 8 bit floats\n");
            } else if(x->is_signed) {
                binary.push_i8((int8_t)x->value.i);
            } else {
                binary.push_u8((uint8_t)x->value.u);
            }

            break;

        case 16:
            if(x->is_float) {
                printf("Internal compiler error: 16 bit floats\n");
            } else if(x->is_signed) {
                binary.push_i16((int16_t)x->value.i);
            } else {
                binary.push_u16((uint16_t)x->value.u);
            }

            break;

        case 32:
            if(x->is_float) {
                binary.push_f32((float)x->value.f);
            } else if(x->is_signed) {
                binary.push_i32((int32_t)x->value.i);
            } else {
                binary.push_u32((uint32_t)x->value.u);
            }

            break;

        case 64:
            if(x->is_float) {
                binary.push_f64(x->value.f);
            } else if(x->is_signed) {
                binary.push_i64(x->value.i);
            } else {
                binary.push_u64(x->value.u);
            }

            break;

        default:
            printf("Internal compiler error: unknown number of bits");
            break;
        }
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

        if(x->value) {
            binary.push_true();
        } else {
            binary.push_false();
        }
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
        binary.push_u32(type_to_size(ele_type) * x->elements.size());
        binary.call("malloc");
        unsigned int offset = 0;

        for(int i = 0; i < x->elements.size(); i++) {
            binary.duplicate();
        }

        for(int i = 0; i < x->elements.size(); i++) {

            generate_il(x->elements[i], il, sem);
            //binary.push_u32(40);
            binary.swap();

            binary.push_u32(offset);
            offset += type_to_size(ele_type);
            binary.integer_add();

            binary.write();
        }
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
        scope->local_add(node);

        binary.function_local( // TODO
            scope_owner.c_str(),
            name.c_str(),
            type_to_il_type(type));

        if(value) {
            generate_il(value, il, sem);
        }

        binary.store_local(name.c_str());
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
