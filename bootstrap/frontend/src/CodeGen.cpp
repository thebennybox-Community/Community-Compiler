
#include "CodeGen.h"
#include <algorithm>

void CodeGen::generateIL(AstNode *node, ILemitter &il) {
    switch(node->node_type) {
    case AstNodeType::AstBlock: {
        auto x = (AstBlock *)node;
        push_scope();
        for(auto y : x->statements) {
            generateIL(y, il);
        }
        pop_scope();
        break;
    }
    case AstNodeType::AstString: {
        auto x = (AstString *)node;
        il.push_str(x->value.c_str());
        break;
    }
    case AstNodeType::AstNumber: {
        auto x = (AstNumber *)node;

        switch(x->bits) {
        case 8:
            if(x->is_float) {
                printf("8 bit floats are not a thing\n");
            } else {
                if(x->is_signed) {
                    il.push_i8(x->value.i);
                } else {
                    il.push_u8(x->value.u);
                }
            }
            break;
        case 16:
            if(x->is_float) {
                printf("16 bit floats are not a thing\n");
            } else {
                if(x->is_signed) {
                    il.push_i16(x->value.i);
                } else {
                    il.push_u16(x->value.u);
                }
            }
            break;
        case 32:
            if(x->is_float) {
                il.push_f32(x->value.f);
            } else {
                if(x->is_signed) {
                    il.push_i32(x->value.i);
                } else {
                    il.push_u32(x->value.u);
                }
            }
            break;
        case 64:
            if(x->is_float) {
                il.push_f64(x->value.f);
            } else {
                if(x->is_signed) {
                    il.push_i64(x->value.i);
                } else {
                    il.push_u64(x->value.u);
                }
            }
            break;
        }

        break;
    }
    case AstNodeType::AstBoolean: {
        auto x = (AstBoolean *)node;
        if(x->value)
            il.push_true();
        else
            il.push_true();
        break;
    }
        /* case AstNodeType::AstArray: {
        auto x = (AstArray *)node;

        break;
    }*/
    case AstNodeType::AstDec: {
        auto x = (AstDec *)node;
        // TODO: dec var in meta block
        add_local(x);

        generateIL(x->value, il);

        il.store_local(x->name->name.c_str());
        break;
    }
    case AstNodeType::AstIf: {
        auto x   = (AstIf *)node;
        auto lbl = (std::string("lbl") + std::to_string(g_counter++)).c_str();
        auto lblout = (std::string(lbl) + std::string("out")).c_str();

        generateIL(x->condition, il);

        il.jump_equal_zero(lbl);

        generateIL(x->true_block, il);
        il.jump(lblout);

        il.label(lbl);

        if(x->false_block != nullptr) {
            generateIL(x->false_block, il);
        }

        il.label(lblout);

        break;
    }
    case AstNodeType::AstFn: {
        auto x = (AstFn *)node;
        if(x->type_self != nullptr) {
            il.function((x->type_self->name + "_" + x->name->name).c_str());
        } else {
            il.function((x->name->name).c_str());
        }

        push_scope();

        for(auto e : x->params) {
            add_arg(e);
        }
        generateIL(x->body, il);

        pop_scope();

        il._return();

        break;
    }
    case AstNodeType::AstFnCall: {
        auto x = (AstFnCall *)node;

        std::reverse(x->args.begin(), x->args.end());

        for(auto y : x->args) {
            generateIL(y, il);
        }

        il.call(x->name->name.c_str());

        break;
    }
    case AstNodeType::AstLoop: {
        auto x = (AstLoop *)node;

        if(x->is_foreach) {
        } else {
            // note to next guy: for and while loops use exact same code gen
            // logic
            auto lbl = (std::string("lbl") + std::to_string(g_counter)).c_str();
            auto lblout =
                (std::string("lblout") + std::to_string(g_counter)).c_str();
            auto lblcont =
                (std::string("lblcont") + std::to_string(g_counter)).c_str();

            g_counter++;

            il.store_local("~");

            il.label(lbl);
            generateIL(x->body, il);

            il.label(lblcont);

            il.load_local("~");
            il.push_u32(1);
            il.integer_add();

            il.duplicate();
            il.store_local("~");

            generateIL(x->expr, il);
            il.integer_subtract();

            il.jump_less_equal_zero(lbl);

            il.label(lblout);
        }

        break;
    }
    case AstNodeType::AstContinue: {
        auto x = (AstContinue *)node;
        auto lblcont =
            (std::string("lblcont") + std::to_string(g_counter)).c_str();

        il.jump(lblcont);

        break;
    }
    case AstNodeType::AstBreak: {
        auto x = (AstBreak *)node;
        auto lblout =
            (std::string("lblout") + std::to_string(g_counter)).c_str();
        il.jump(lblout);

        break;
    }
        /* case AstNodeType::AstStruct: {
        auto x = (AstStruct *)node;

        break;
    }
    */
    case AstNodeType::AstImpl: {
        auto x = (AstImpl *)node;
        generateIL(x->block, il);
        break;
    }
        /*   case AstNodeType::AstAttribute: {
        auto x = (AstAttribute *)node;

        break;
    }*/
    case AstNodeType::AstAffix: {
        auto x = (AstAffix *)node;

        il.function(x->name->name.c_str());

        generateIL(x->body, il);

        il._return();
        break;
    }
    case AstNodeType::AstUnaryExpr: {
        auto x = (AstUnaryExpr *)node;
        generateIL(x->expr, il);
        il.call(x->op.c_str());
        break;
    }
    case AstNodeType::AstBinaryExpr: {
        auto x = (AstBinaryExpr *)node;
        generateIL(x->rhs, il);
        generateIL(x->lhs, il);
        il.call(x->op.c_str());
        break;
    }
    case AstNodeType::AstIndex: {
        auto x = (AstIndex *)node;

        break;
    }
        /* case AstNodeType::AstType: {
        auto x = (AstType *)node;

        break;
    }*/
    case AstNodeType::AstSymbol: {
        auto x = (AstSymbol *)node;
        if(has_local(x->name)) {
            il.load_local(x->name.c_str());
        } else {
            if(has_arg(x->name)) {
                il.load_argument(x->name.c_str());
            } else {
                il.push_function(x->name.c_str());
            }
        }
        break;
    }
    case AstNodeType::AstReturn: {
        auto x = (AstReturn *)node;

        if(x->expr != nullptr) {
            generateIL(x->expr, il);
        }
        il._return();

        break;
    }
    case AstNodeType::AstExtern: {
        auto x = (AstExtern *)node;

        break;
    }
    }
}