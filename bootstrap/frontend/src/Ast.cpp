#include "CodeGen.h"

#include <string>
#include <stdint.h>
#include "Ast.h"

using namespace std::literals::string_literals;

static unsigned char type_to_il_type(const AstType *type)
{
    if (!type)
    {
        return VOID;
    }

    if (type->is_array)
    {
        return type_to_il_type(type->subtype);
    }

    return type_map.at(type->name);
}

static std::string type_to_string(const AstType *type)
{
    if (type->is_array)
    {
        return type_to_string(type->subtype) + "Arr";
    }

    return type->name;
}

void AstBlock::code_gen(ILemitter &il, Semantics &sem)
{
    push_scope();
    g_counter++;

    for (auto stmt : statements)
    {
        generate_il(stmt, il, sem);
    }

    g_counter++;
    pop_scope();
}

void AstString::code_gen(ILemitter &il, Semantics &sem)
{
    (void)sem;
    il.push_str(value.c_str());
}

void AstNumber::code_gen(ILemitter &il, Semantics &sem)
{
    switch (bits)
    {
    case 8:
        if (is_float)
        {
            printf("Internal compiler error: 8 bit floats\n");
        }
        else if (is_signed)
        {
            il.push_i8((int8_t)value.i);
        }
        else
        {
            il.push_u8((uint8_t)value.u);
        }

        break;

    case 16:
        if (is_float)
        {
            printf("Internal compiler error: 16 bit floats\n");
        }
        else if (is_signed)
        {
            il.push_i16((int16_t)value.i);
        }
        else
        {
            il.push_u16((uint16_t)value.u);
        }

        break;

    case 32:
        if (is_float)
        {
            il.push_f32((float)value.f);
        }
        else if (is_signed)
        {
            il.push_i32((int32_t)value.i);
        }
        else
        {
            il.push_u32((uint32_t)value.u);
        }

        break;

    case 64:
        if (is_float)
        {
            il.push_f64(value.f);
        }
        else if (is_signed)
        {
            il.push_i64(value.i);
        }
        else
        {
            il.push_u64(value.u);
        }

        break;

    default:
        printf("Internal compiler error: unknown number of bits");
        break;
    }
}

void AstBoolean::code_gen(ILemitter &il, Semantics &sem)
{
    (void)sem;
    if (value)
    {
        il.push_true();
    }
    else
    {
        il.push_false();
    }
}

void AstArray::code_gen(ILemitter &il, Semantics &sem)
{
    (void)il;
    (void)sem;
}

void AstDec::code_gen(ILemitter &il, Semantics &sem)
{
    add_local(this);

    il.function_local( // TODO
        scope_owner.c_str(),
        name.c_str(),
        type_to_il_type(type));

    if (value)
    {
        generate_il(value, il, sem);
    }

    il.store_local(name.c_str());
}

void AstIf::code_gen(ILemitter &il, Semantics &sem)
{
    //auto buf = g_counter;
    g_counter++;

    auto lbl = "lbl"s + std::to_string(g_counter);
    auto lblout = "lblout"s + std::to_string(g_counter);

    generate_il(condition, il, sem);

    il.jump_equal_zero(lbl.c_str());

    generate_il(true_block, il, sem);
    il.jump(lblout.c_str());

    il.label(lbl.c_str());

    if (false_block)
    {
        generate_il(false_block, il, sem);
    }

    il.label(lblout.c_str());
    g_counter++;

    // g_counter = buf;
}

void AstFn::code_gen(ILemitter &il, Semantics &sem)
{
    scope_owner = mangled_name;

    // if(body != nullptr) {
    //     for(auto a : params) {
    //         mangled_name += type_to_string(a->type);
    //     }
    // }

    if (body)
    {
        for (auto param : params)
        {
            il.function_parameter(
                mangled_name.c_str(),
                param->name.c_str(),
                type_to_il_type(param->type));
        }

        il.internal_function(
            mangled_name.c_str(), type_to_il_type(return_type));
    }
    else
    {
        unsigned char args[params.size()];

        for (size_t i = 0; i < params.size(); i++)
        {
            args[i] = type_to_il_type(params[i]->type);
        }

        il.external_function(
            unmangled_name.c_str(),
            type_to_il_type(return_type),
            (uint32_t)params.size(),
            args);
    }

    if (body)
    {
        if (type_self != "")
        {
            il.function((type_self + "_" + mangled_name).c_str());
        }
        else
        {
            il.function(mangled_name.c_str());
        }

        push_scope();

        for (auto param : params)
        {
            add_arg(param);
        }

        if (attributes.size() == 0)
        {
            generate_il(body, il, sem);
        }
        else
        {
            for (auto attribute : attributes)
            {
                if (attribute->name == "il")
                {
                    for (auto stmt : body->statements)
                    {
                        if (stmt->node_type == AstNodeType::AstNumber)
                        {
                            auto number = (AstNumber *)stmt;

                            if (number->is_signed)
                            {
                                il.w((unsigned char)number->value.i);
                            }
                            else
                            {
                                il.w((char)number->value.u);
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

void AstFnCall::code_gen(ILemitter &il, Semantics &sem)
{
    auto fn = sem.p2_get_fn(name);

    for (size_t i = args.size(); i; i--)
    {
        generate_il(args[i - 1], il, sem);
    }

    if (fn->attributes.size() == 0)
    {
        il.call(name.c_str());
    }
    else
    {
        for (auto attribute : fn->attributes)
        {
            if (attribute->name == "il")
            {
                for (auto stmt : fn->body->statements)
                {
                    if (stmt->node_type == AstNodeType::AstNumber)
                    {
                        auto number = (AstNumber *)stmt;

                        if (number->is_signed)
                        {
                            il.w((uint8_t)number->value.i);
                        }
                        else
                        {
                            il.w((int8_t)number->value.u);
                        }
                    }
                }

                break;
            }
        }
    }
}

void AstLoop::code_gen(ILemitter &il, Semantics &sem)
{
    auto type = sem.infer_type(expr);

    // auto buf = g_counter;
    g_counter++;

    if (is_foreach)
    {
        // TODO: need enumerables for this
    }
    else if (type && type->name != "bool")
    {
        auto var = "~"s + std::to_string(g_counter++);

        il.function_local(scope_owner.c_str(), var.c_str(), U32);

        auto lbl = "lbl"s + std::to_string(g_counter);
        auto lblout = "lblout"s + std::to_string(g_counter);
        auto lblcont = "lblcont"s + std::to_string(g_counter);

        il.push_u32(1);
        il.store_local(var.c_str());

        // il.jump(lblcont.c_str());

        il.label(lbl.c_str());
        generate_il(body, il, sem);

        il.label(lblcont.c_str());

        il.load_local(var.c_str());
        il.push_u32(1);
        il.integer_add();

        il.duplicate();
        il.store_local(var.c_str());

        generate_il(expr, il, sem);
        il.integer_subtract();

        il.jump_less_equal_zero(lbl.c_str());

        il.label(lblout.c_str());
        g_counter++;
    }
    else
    {
        auto lbl = "lbl"s + std::to_string(g_counter);
        auto lbl_cond = "lbl_cond"s + std::to_string(g_counter);

        il.jump(lbl_cond.c_str());

        il.label(lbl.c_str());
        generate_il(body, il, sem);

        il.label(lbl_cond.c_str());
        generate_il(expr, il, sem);

        il.push_i8(1);
        il.integer_subtract();
        il.jump_equal_zero(lbl.c_str());

        g_counter++;
    }

    delete type;

    //  g_counter = buf;
}

void AstContinue::code_gen(ILemitter &il, Semantics &sem)
{
    (void)sem;
    auto lblcont = ("lblcont"s + std::to_string(g_counter)).c_str();
    il.jump(lblcont);
}

void AstBreak::code_gen(ILemitter &il, Semantics &sem)
{
    (void)sem;
    auto lblout = ("lblout"s + std::to_string(g_counter)).c_str();
    il.jump(lblout);
}

void AstStruct::code_gen(ILemitter &il, Semantics &sem)
{
    (void)il;
    (void)sem;
}

void AstImpl::code_gen(ILemitter &il, Semantics &sem)
{
    generate_il(block, il, sem);
}

void AstAttribute::code_gen(ILemitter &il, Semantics &sem)
{
}

void AstAffix::code_gen(ILemitter &il, Semantics &sem)
{
    for (auto param : params)
    {
        name += type_to_string(param->type);
    }

    il.internal_function(name.c_str(), type_to_il_type(return_type));

    il.function(name.c_str());

    generate_il(body, il, sem);

    il._return();
}

void AstUnaryExpr::code_gen(ILemitter &il, Semantics &sem)
{
    generate_il(expr, il, sem);
    il.call(op.c_str());
}

void AstBinaryExpr::code_gen(ILemitter &il, Semantics &sem)
{
    if (op == "=")
    {
        generate_il(rhs, il, sem);

        if (lhs->node_type == AstNodeType::AstIndex)
        {
            generate_il(lhs, il, sem);
            il.remove_last();
            il.write();
        }
        else if (lhs->node_type == AstNodeType::AstSymbol)
        {
            auto x = (AstSymbol *)lhs;
            auto local = get_local(x->name);

            if (local && !local->immutable)
            {
                il.store_local(x->name.c_str());
            }
            else
            {
                printf("You can not assign a value to an immutable. \n");
            }
        }
        else
        {
            generate_il(lhs, il, sem);
            il.address_stack();
            il.write();
        }
        return;
    }

    generate_il(lhs, il, sem);
    generate_il(rhs, il, sem);
    // il.call(op.c_str());

    {
        auto fn = sem.p2_get_fn(op);

        if (fn)
        {
            for (auto stmt : fn->body->statements)
            {
                generate_il(stmt, il, sem);
            }
        }
    }
    {
        auto fn = sem.p2_get_affix(op);

        if (fn)
        {
            for (auto stmt : fn->body->statements)
            {
                generate_il(stmt, il, sem);
            }
        }
    }
}

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

int type_to_size(AstType *type)
{
    if (type->is_array)
    {
        type_to_size(type->subtype);
    }

    return type_size_map.at(type->name);
}

void AstIndex::code_gen(ILemitter &il, Semantics &sem)
{
    generate_il(array, il, sem);

    auto type = sem.infer_type(array);
    auto size = type_to_size(type);

    il.address_stack();

    il.push_i32(type_to_size(type));

    generate_il(expr, il, sem);

    il.integer_multiply();
    il.integer_add();

    il.read();

    delete type;
}

void AstType::code_gen(ILemitter &il, Semantics &sem)
{
    (void)il;
    (void)sem;
}

void AstSymbol::code_gen(ILemitter &il, Semantics &sem)
{
    (void)sem;
    if (has_local(name))
    {
        il.load_local(name.c_str());
    }
    else if (has_arg(name))
    {
        il.load_argument(name.c_str());
    }
    else
    {
        il.push_function(name.c_str());
    }
}

void AstReturn::code_gen(ILemitter &il, Semantics &sem)
{
    if (expr)
    {
        generate_il(expr, il, sem);
    }

    il._return();
}

void AstExtern::code_gen(ILemitter &il, Semantics &sem)
{
    for (auto decl : decls)
    {
        generate_il(decl, il, sem);
    }
}
