#include "ICodeGenerator.h"
#include "Ast.h"

static unsigned int g_counter = 0;


static const std::map<std::string, int> type_size_map = {
    {"u8", 1},
    {"bool", 1},
    {"u16", 2},
    {"u32", 4},
    {"u64", 8},
    {"i8", 1},
    {"i16", 2},
    {"i32", 4},
    {"i64", 8},
    {"f32", 4},
    {"f64", 8},
    {"str", 1},
    {"ptr", 4},
    {"void", 1},
};

static int type_to_size(AstType *type) {
    if(!type || type->is_array) {
        return type_to_size(type->subtype);
    }

    return type_size_map.at(type->name);
}

static unsigned char type_to_il_type(const AstType *type) {
    if(!type) {
        return VOID;
    }

    if(type->is_array) {
        return type_to_il_type(type->subtype);
    }

    for(auto x : type_map) {
        if(x.first == type->name) {
            return type_map.at(type->name);
        }
    }

    return U32;
}

static std::string type_to_string(const AstType *type) {
    if(type->is_array) {
        return type_to_string(type->subtype) + "Arr";
    }

    return type->name;
}

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
        auto x = (AstBoolean *)node;

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
        auto x = (AstArray *)node;

        binary.push_u32(type_to_size(x->ele_type) * x->elements.size());
        binary.call("malloc");
        unsigned int offset = 0;

        for(int i = 0; i < x->elements.size(); i++) {
            binary.duplicate();
        }

        for(int i = 0; i < x->elements.size(); i++) {

            ds.generate_code_node(x->elements[i]);
            //binary.push_u32(40);
            binary.swap();

            binary.push_u32(offset);
            offset += type_to_size(x->ele_type);
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
        auto x = (AstDec *)node;
        scope->local_add(x);

        binary.function_local( // TODO
            scope->scope_stack.front()->owner_name.c_str(),
            x->name.c_str(),
            type_to_il_type(x->type));

        if(x->value) {
            ds.generate_code_node(x->value);
        }

        binary.store_local(x->name.c_str());
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
        auto x = (AstFn *)node;

        if(x->body) {
            //internal method

            for(auto param : x->params) {
                binary.function_parameter(
                    x->name.c_str(), //TODO: Mnagle that shit
                    param->name.c_str(),
                    type_to_il_type(param->type));

                scope->arg_add(param);
            }

            binary.internal_function(
                x->name.c_str(), type_to_il_type(x->return_type));

            binary.function(x->name.c_str());
        } else {
            //external method

            unsigned char args[x->params.size()];

            for(size_t i = 0; i < x->params.size(); i++) {
                args[i] = type_to_il_type(x->params[i]->type);
            }

            binary.external_function(
                x->name.c_str(),
                type_to_il_type(x->return_type),
                (uint32_t)x->params.size(),
                args);
        }
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

        auto x = (AstFnCall *)node;
        auto types = std::vector<AstType *>();

        for(auto arg : x->args) {
            types.push_back(scope->infer_type(arg));
        }

        auto target = scope->func_get(x->name, types);

        for(size_t i = x->args.size(); i; i--) {
            auto z = x->args[i - 1];
            ds.generate_code_node(z);
        }

        if(target->body) {
            //internal
            //dont mangle shit

        } else {
            //external
            //we need to mangle that up sone
        }

        binary.call(x->name.c_str());

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
        auto x = (AstBinaryExpr *)node;

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
        auto x = (AstSymbol *)node;
        {
            auto z = scope->struct_get(x->name);

            if(z) {
                //todo
            }
        }
        {
            auto z = scope->global_get(x->name);

            if(z) {
                //todo
            }
        }
        {
            auto z = scope->arg_get(x->name);

            if(z) {
                binary.load_argument(z->name.c_str());
            }
        }
        {
            auto z = scope->local_get(x->name);

            if(z) {
                binary.load_local(z->name.c_str());
            }
        }
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
        auto x = (AstReturn *)node;
        ds.generate_code_node(x->expr);
        binary._return();
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
