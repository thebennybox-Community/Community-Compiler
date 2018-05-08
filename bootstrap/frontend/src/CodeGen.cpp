
#include "CodeGen.h"
#include <algorithm>

static std::string type_to_string(const AstType *node) {
    if(node->is_array) {
        return type_to_string(node->subtype) + "Arr";
    }

    return node->name;
}

void CodeGen::generateIL(AstNode *node, ILemitter &il) {
    if(!node->emit) {
        return;
    }

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

        if(x->value) {
            il.push_true();
        } else {
            il.push_true();
        }

        break;
    }

    /* case AstNodeType::AstArray: {
    auto x = (AstArray *)node;

    break;
    }*/
    case AstNodeType::AstDec: {
        auto x = (AstDec *)node;

        add_local(x);

        il.FunctionLocal(
            scope_owner->name.c_str(),
            x->name->name.c_str(),
            to_IL_type(x->type));

        generateIL(x->value, il);

        il.store_local(x->name->name.c_str());
        break;
    }

    case AstNodeType::AstIf: {
        auto x      = (AstIf *)node;
        auto lbl    = std::string("lbl") + std::to_string(g_counter);
        auto lblout = std::string("lblout") + std::to_string(g_counter);

        generateIL(x->condition, il);

        il.jump_equal_zero(lbl.c_str());

        generateIL(x->true_block, il);
        il.jump(lblout.c_str());

        il.label(lbl.c_str());

        if(x->false_block != nullptr) {
            generateIL(x->false_block, il);
        }

        il.label(lblout.c_str());
        g_counter++;
        break;
    }

    case AstNodeType::AstFn: {
        auto x = (AstFn *)node;

        auto buf = x->name->name;

        if(x->body != nullptr) {
            for(auto a : x->params) {
                x->name->name += type_to_string(a->type);
            }
        }

        scope_owner = x->name;

        if(x->body != nullptr) {
            for(auto p : x->params) {
                il.FunctionParameter(
                    x->name->name.c_str(),
                    p->name->name.c_str(),
                    to_IL_type(p->type));
            }

            il.InternalFunction(
                (x->name->name).c_str(), to_IL_type(x->return_type));
        } else {
            unsigned char args[x->params.size()];

            for(int i = 0; i < x->params.size(); i++) {
                auto b  = (x->params.at(i)->type);
                args[i] = to_IL_type(b);
            }

            il.ExternalFunction(
                (x->name->name).c_str(),
                to_IL_type(x->return_type),
                x->params.size(),
                args);
        }

        if(x->body != nullptr) {
            if(x->type_self != nullptr) {
                il.function((x->type_self->name + "_" + x->name->name).c_str());
            } else {
                il.function(x->name->name.c_str());
            }

            push_scope();

            for(auto e : x->params) {
                add_arg(e);
            }

            if(x->nested_attributes.size() == 0) {
                generateIL(x->body, il);
            } else {
                for(auto y : x->nested_attributes) {
                    if(y->name->name == "il") {
                        for(auto b : x->body->statements) {
                            if(b->node_type == AstNodeType::AstNumber) {
                                auto n = (AstNumber *)b;

                                if(n->is_signed) {
                                    il.w((unsigned char)n->value.i);
                                } else {
                                    il.w((char)n->value.u);
                                }
                            }
                        }

                        break;
                    }
                }
            }

            pop_scope();

            il._return();
            x->name->name = buf;
        }

        break;
    }

    case AstNodeType::AstFnCall: {

        auto x = (AstFnCall *)node;

        std::reverse(x->args.begin(), x->args.end());

        for(auto y : x->args) {
            generateIL(y, il);
        }

        auto fn  = sem.p2_get_fn(x->name);
        auto buf = x->name->name;

        for(auto a : fn->params) {
            x->name->name += type_to_string(a->type);
        }

        if(fn->nested_attributes.size() == 0) {
            il.call(x->name->name.c_str());
        } else {
            for(auto y : fn->nested_attributes) {
                if(y->name->name == "il") {
                    for(auto b : fn->body->statements) {
                        if(b->node_type == AstNodeType::AstNumber) {
                            auto n = (AstNumber *)b;

                            if(n->is_signed) {
                                il.w((unsigned char)n->value.i);
                            } else {
                                il.w((char)n->value.u);
                            }
                        }
                    }

                    break;
                }
            }
        }

        x->name->name = buf;

        break;
    }

    case AstNodeType::AstLoop: {
        auto x = (AstLoop *)node;

        if(x->is_foreach) {
        } else {

            il.FunctionLocal(scope_owner->name.c_str(), "~", U32);
            // note to next guy: for and while loops use exact same code gen
            // logic
            auto lbl     = std::string("lbl") + std::to_string(g_counter);
            auto lblout  = std::string("lblout") + std::to_string(g_counter);
            auto lblcont = std::string("lblcont") + std::to_string(g_counter);

            il.push_u32(1);
            il.store_local("~");

            // il.jump(lblcont.c_str());

            il.label(lbl.c_str());
            generateIL(x->body, il);

            il.label(lblcont.c_str());

            il.load_local("~");
            il.push_u32(1);
            il.integer_add();

            il.duplicate();
            il.store_local("~");

            generateIL(x->expr, il);
            il.integer_subtract();

            il.jump_less_equal_zero(lbl.c_str());

            il.label(lblout.c_str());
            g_counter++;
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

        for(auto a : x->params) {
            x->name->name += type_to_string(a->type);
        }

        il.InternalFunction(
            (x->name->name).c_str(), to_IL_type(x->return_type));

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
        generateIL(x->lhs, il);
        generateIL(x->rhs, il);
        // il.call(x->op.c_str());

        {
            auto fn = sem.p2_get_fn(x->op);

            if(fn != nullptr) {
                for(auto x : fn->body->statements) {
                    generateIL(x, il);
                }
            }
        }

        {
            auto fn = sem.p2_get_affix(x->op);

            if(fn != nullptr) {
                for(auto x : fn->body->statements) {
                    generateIL(x, il);
                }
            }
        }

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

        for(auto b : x->decls) {
            generateIL(b, il);
        }

        break;
    }
    }
}