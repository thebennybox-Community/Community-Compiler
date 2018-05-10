#include "CodeGen.h"
#include "Ast.h"

#include <algorithm>

Ast::~Ast() {
    delete root;
}

static unsigned char to_IL_type(AstType *x) {
    if(x == nullptr) {
        return VOID;
    }

    if(x->is_array) {
        return to_IL_type(x->subtype);
    }

    return type_map.at(x->name);
}

static std::string type_to_string(const AstType *node) {
    if(node->is_array) {
        return type_to_string(node->subtype) + "Arr";
    }

    return node->name;
}

void AstBlock::code_gen(ILemitter &il, Semantics &sem) {
    push_scope();

    for(auto y : statements) {
        generateIL(y, il, sem);
    }

    pop_scope();
}

void AstString::code_gen(ILemitter &il, Semantics &sem) {
    il.push_str(value.c_str());
}

void AstNumber::code_gen(ILemitter &il, Semantics &sem) {
    switch(bits) {
    case 8:
        if(is_float) {
            printf("8 bit floats are not a thing\n");
        } else {
            if(is_signed) {
                il.push_i8((char)value.i);
            } else {
                il.push_u8((unsigned char)value.u);
            }
        }

        break;

    case 16:
        if(is_float) {
            printf("16 bit floats are not a thing\n");
        } else {
            if(is_signed) {
                il.push_i16((short)value.i);
            } else {
                il.push_u16((unsigned short)value.u);
            }
        }

        break;

    case 32:
        if(is_float) {
            il.push_f32((float)value.f);
        } else {
            if(is_signed) {
                il.push_i32((int)value.i);
            } else {
                il.push_u32((unsigned int)value.u);
            }
        }

        break;

    case 64:
        if(is_float) {
            il.push_f64(value.f);
        } else {
            if(is_signed) {
                il.push_i64(value.i);
            } else {
                il.push_u64(value.u);
            }
        }

        break;
    }

}

void AstBoolean::code_gen(ILemitter &il, Semantics &sem) {
    if(value) {
        il.push_true();
    } else {
        il.push_true();
    }

}

void AstArray::code_gen(ILemitter &il, Semantics &sem) {

}

void AstDec::code_gen(ILemitter &il, Semantics &sem) {

    add_local(this);

    il.FunctionLocal(
        scope_owner->name.c_str(),
        name->name.c_str(),
        to_IL_type(type));

    generateIL(value, il, sem);

    il.store_local(name->name.c_str());
}

void AstIf::code_gen(ILemitter &il, Semantics &sem) {
    auto lbl    = std::string("lbl") + std::to_string(g_counter);
    auto lblout = std::string("lblout") + std::to_string(g_counter);

    generateIL(condition, il, sem);

    il.jump_equal_zero(lbl.c_str());

    generateIL(true_block, il, sem);
    il.jump(lblout.c_str());

    il.label(lbl.c_str());

    if(false_block != nullptr) {
        generateIL(false_block, il, sem);
    }

    il.label(lblout.c_str());
    g_counter++;
}

void AstFn::code_gen(ILemitter &il, Semantics &sem) {

    scope_owner = mangled_name;

    // if(body != nullptr) {
    //     for(auto a : params) {
    //         mangled_name->name += type_to_string(a->type);
    //     }
    // }

    if(body != nullptr) {
        for(auto p : params) {
            il.FunctionParameter(
                mangled_name->name.c_str(),
                p->name->name.c_str(),
                to_IL_type(p->type));
        }

        il.InternalFunction(
            (mangled_name->name).c_str(), to_IL_type(return_type));
    } else {
        unsigned char args[params.size()];

        for(unsigned int i = 0; i < params.size(); i++) {
            auto b  = (params.at(i)->type);
            args[i] = to_IL_type(b);
        }

        il.ExternalFunction(
            (unmangled_name->name).c_str(),
            to_IL_type(return_type),
            (unsigned int)params.size(),
            args);
    }

    if(body != nullptr) {
        if(type_self != nullptr) {
            il.function((type_self->name + "_" + mangled_name->name).c_str());
        } else {
            il.function(mangled_name->name.c_str());
        }

        push_scope();

        for(auto e : params) {
            add_arg(e);
        }

        if(nested_attributes.size() == 0) {
            generateIL(body, il, sem);
        } else {
            for(auto y : nested_attributes) {
                if(y->name->name == "il") {
                    for(auto b : body->statements) {
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
    }
}

void AstFnCall::code_gen(ILemitter &il, Semantics &sem) {
    auto fn  = sem.p2_get_fn(name);

    std::reverse(args.begin(), args.end());

    for(auto y : args) {
        generateIL(y, il, sem);
    }


    auto buf = name->name;

    if(fn->nested_attributes.size() == 0) {
        il.call(name->name.c_str());
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

    name->name = buf;
}

void AstLoop::code_gen(ILemitter &il, Semantics &sem) {

    auto x = sem.determin_type(expr);

    if(is_foreach) {
    } else if(x != nullptr && x->name != "bool") {

        auto var = "~" + std::to_string(g_counter++);

        il.FunctionLocal(scope_owner->name.c_str(), var.c_str(), U32);
        // note to next guy: for and while loops use exact same code gen
        // logic
        auto lbl     = std::string("lbl") + std::to_string(g_counter);
        auto lblout  = std::string("lblout") + std::to_string(g_counter);
        auto lblcont = std::string("lblcont") + std::to_string(g_counter);

        il.push_u32(1);
        il.store_local(var.c_str());

        // il.jump(lblcont.c_str());

        il.label(lbl.c_str());
        generateIL(body, il, sem);

        il.label(lblcont.c_str());

        il.load_local(var.c_str());
        il.push_u32(1);
        il.integer_add();

        il.duplicate();
        il.store_local(var.c_str());

        generateIL(expr, il, sem);
        il.integer_subtract();

        il.jump_less_equal_zero(lbl.c_str());

        il.label(lblout.c_str());
        g_counter++;
    } else {
        auto lbl     = std::string("lbl") + std::to_string(g_counter);
        auto lbl_cond     = std::string("lbl_cond") + std::to_string(g_counter);

        il.jump(lbl_cond.c_str());

        il.label(lbl.c_str());
        generateIL(body, il, sem);

        il.label(lbl_cond.c_str());
        generateIL(expr, il, sem);

        il.push_i8(1);
        il.integer_subtract();
        il.jump_equal_zero(lbl.c_str());

        g_counter++;
    }
}

void AstContinue::code_gen(ILemitter &il, Semantics &sem) {

    auto lblcont =
        (std::string("lblcont") + std::to_string(g_counter)).c_str();

    il.jump(lblcont);

}

void AstBreak::code_gen(ILemitter &il, Semantics &sem) {

    auto lblout =
        (std::string("lblout") + std::to_string(g_counter)).c_str();
    il.jump(lblout);
}

void AstStruct::code_gen(ILemitter &il, Semantics &sem) {
}

void AstImpl::code_gen(ILemitter &il, Semantics &sem) {


    generateIL(block, il, sem);
}

void AstAttribute::code_gen(ILemitter &il, Semantics &sem) {

}

void AstAffix::code_gen(ILemitter &il, Semantics &sem) {
    for(auto a : params) {
        name->name += type_to_string(a->type);
    }

    il.InternalFunction(
        (name->name).c_str(), to_IL_type(return_type));

    il.function(name->name.c_str());

    generateIL(body, il, sem);

    il._return();
}

void AstUnaryExpr::code_gen(ILemitter &il, Semantics &sem) {
    generateIL(expr, il, sem);
    il.call(op.c_str());
}

void AstBinaryExpr::code_gen(ILemitter &il, Semantics &sem) {
    if(op == "=") {
        generateIL(rhs, il, sem);

        auto x = (AstSymbol *)lhs;

        if(!get_local(x->name)->immutable) {
            il.store_local(x->name.c_str());
        } else {
            printf("You can not assign a value to an immutable. \n");
        }

        return;
    }


    generateIL(lhs, il, sem);
    generateIL(rhs, il, sem);
    // il.call(op.c_str());




    {
        auto fn = sem.p2_get_fn(op);

        if(fn != nullptr) {
            for(auto x : fn->body->statements) {
                generateIL(x, il, sem);
            }
        }
    }

    {
        auto fn = sem.p2_get_affix(op);

        if(fn != nullptr) {
            for(auto x : fn->body->statements) {
                generateIL(x, il, sem);
            }
        }
    }

}

void AstIndex::code_gen(ILemitter &il, Semantics &sem) {

}

void AstType::code_gen(ILemitter &il, Semantics &sem) {

}

void AstSymbol::code_gen(ILemitter &il, Semantics &sem) {

    if(has_local(name)) {
        il.load_local(name.c_str());
    } else {
        if(has_arg(name)) {
            il.load_argument(name.c_str());
        } else {
            il.push_function(name.c_str());
        }
    }
}

void AstReturn::code_gen(ILemitter &il, Semantics &sem) {

    if(expr != nullptr) {
        generateIL(expr, il, sem);
    }

    il._return();
}

void AstExtern::code_gen(ILemitter &il, Semantics &sem) {

    for(auto b : decls) {
        generateIL(b, il, sem);
    }
}
