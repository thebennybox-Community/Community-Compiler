#include "Semantics.h"

#include <string>
#include "Ast.h"
#include "CodeGen.h"

using namespace std::literals::string_literals;

static AstType *clone_type(const AstType *type) {
    auto clone = new AstType();
    auto result = clone;
    clone->name = type->name;
    while(type->subtype) {
        clone->subtype = new AstType();
        clone->subtype->name = type->subtype->name;
        type  = type->subtype;
        clone = clone->subtype;
    }
    return result;
}

bool Semantics::p1_has_symbol(const std::string &symbol) {
    for(auto sym : p1_funcs) {
        if(sym == symbol) {
            return true;
        }
    }

    for(auto sym : p1_structs) {
        if(sym == symbol) {
            return true;
        }
    }

    return false;
}

bool Semantics::p1_has_symbol(const AstType *type) {
    if(!type) {
        return false;
    }

    for(auto sym : p1_funcs) {
        if(sym == type->name) {
            return true;
        }
    }

    for(auto sym : p1_structs) {
        if(sym == type->name) {
            return true;
        }
    }

    return p1_has_symbol(type->subtype);
}

AstFn *Semantics::p2_get_fn(const AstSymbol *name) {
    return p2_get_fn(name->name);
}

AstFn *Semantics::p2_get_fn(const std::string &name) {
    for(auto sym : p2_funcs) {
        if(sym->mangled_name == name) {
            return sym;
        }
    }

    return nullptr;
}

AstFn *Semantics::p2_get_fn_unmangled(const std::string &name) {
    for(auto sym : p2_funcs) {
        if(sym->unmangled_name == name) {
            return sym;
        }
    }

    return nullptr;
}

AstFn *Semantics::p2_get_fn_unmangled(const AstSymbol *name) {
    return p2_get_fn_unmangled(name->name);
}

AstAffix *Semantics::p2_get_affix(const AstSymbol *name) {
    return p2_get_affix(name->name);
}

AstAffix *Semantics::p2_get_affix(const std::string &name) {
    for(auto sym : p2_affixes) {
        if(sym->name == name) {
            return sym;
        }
    }

    return nullptr;
}

/*AstDec *Semantics::p2_get_dec(const AstSymbol *name) {
    for(auto sym : p2_dec) {
        if(sym->name == name->name) {
            return sym;
        }
    }

    return nullptr;
}

AstDec *Semantics::p2_get_dec(const std::string &name) {
    for(auto sym : p2_dec) {
        if(sym->name == name) {
            return sym;
        }
    }

    return nullptr;
}*/

void Semantics::p1_fn(AstFn *node) {
    p1_funcs.push_back(node->mangled_name);
}

void Semantics::p1_struct(AstStruct *node) {
    p1_structs.push_back(node->name);
}

void Semantics::pass1_node(AstNode *node) {
    switch(node->node_type) {
    case AstNodeType::AstFn:
        p1_fn((AstFn*)node);
        break;

    case AstNodeType::AstAffix:
        p1_funcs.push_back(((AstAffix*)node)->name);
        break;

    case AstNodeType::AstStruct:
        p1_struct((AstStruct*)node);
        break;

    case AstNodeType::AstBlock:
        for(auto stmt : ((AstBlock*)node)->statements) {
            pass1_node(stmt);
        }
        break;

    case AstNodeType::AstImpl:
        for(auto stmt : ((AstImpl*)node)->block->statements) {
            if(stmt->node_type == AstNodeType::AstFn) {
                ((AstFn*)stmt)->type_self = ((AstImpl*)node)->name;
            }
        }

        pass1_node(((AstImpl*)node)->block);
        break;

    case AstNodeType::AstExtern:
        for(auto decl : ((AstExtern*)node)->decls) {
            pass1_node(decl);
        }
        break;

    default:
        break;
    }
}

void Semantics::pass1(Ast &ast) {
    pass1_node(ast.root);
}

void Semantics::pass2(Ast &ast) {
    pass2_node(ast.root);
}

void Semantics::pass2_node(AstNode *node) {
    switch(node->node_type) {
    case AstNodeType::AstFn:
        p2_fn((AstFn*)node);
        break;

    case AstNodeType::AstAffix:
        p2_affix((AstAffix*)node);
        break;

    case AstNodeType::AstStruct:
        p2_struct((AstStruct*)node);
        break;

    case AstNodeType::AstBlock:
        for(auto stmt : ((AstBlock *)node)->statements) {
            pass2_node(stmt);
        }
        break;

    case AstNodeType::AstImpl:
        pass2_node(((AstImpl*)node)->block);
        break;

    case AstNodeType::AstExtern:
        for(auto decl : ((AstExtern*)node)->decls) {
            pass2_node(decl);
        }
        break;

    default:
        break;
    }
}

static std::string type_to_string(const AstType *node) {
    if(node->is_array) {
        return type_to_string(node->subtype) + "Arr";
    }

    return node->name;
}

void Semantics::p2_affix(AstAffix *node) {
    /*for(auto a : node->params) {
        // node->name += type_to_string(a->type);
    }*/

    if(node->return_type) {
        if(!p1_has_symbol(node->return_type)) {
            this->errors.emplace_back(
                ErrorType::TypeNotFound, node->return_type,
                "Type does not exist"
            );
            return;
        }
    }

    for(auto param : node->params) {
        if(param->type) {
            if(!p1_has_symbol(param->type)) {
                this->errors.emplace_back(
                    ErrorType::TypeNotFound, param->type,
                    "Type does not exist"
                );
                return;
            }
        }
    }

    /*if(node->body) {
        for(auto a : node->params) {
            // node->name += type_to_string(a->type);
        }
    }*/

    p2_affixes.push_back(node);
}

void Semantics::p2_fn(AstFn *node) {
    if(node->body) {
        for(auto param : node->params) {
            node->mangled_name += type_to_string(param->type);
        }
    }

    if(node->type_self != "") {
        if(!p1_has_symbol(node->type_self)) {
            this->errors.emplace_back(
                ErrorType::TypeNotFound, node,
                "Type does not exist"
            );
            return;
        }

        // we need to add implicit self
        AstDec *self = new AstDec();
        self->name   = "self";

        self->type       = new AstType();
        self->type->name = node->type_self;

        node->params.insert(node->params.begin(), self);
    }

    if(node->return_type) {
        if(!p1_has_symbol(node->return_type)) {
            this->errors.emplace_back(
                ErrorType::TypeNotFound, node->return_type,
                "Type does not exist"
            );
            return;
        }
    }

    for(auto param : node->params) {
        if(param->type) {
            if(!p1_has_symbol(param->type)) {
                this->errors.emplace_back(
                    ErrorType::TypeNotFound, param->type,
                    "Type does not exist"
                );
                return;
            }
        }
    }

    /*if(node->body) {
        for(auto a : node->params) {
            // node->name += type_to_string(a->type);
        }
    }*/

    p2_funcs.push_back(node);
}

void Semantics::p2_struct(AstStruct *node) {
    for(auto stmt : node->block->statements) {
        if(stmt->node_type == AstNodeType::AstDec) {
            if(((AstDec*)stmt)->type) {
                if(!p1_has_symbol(((AstDec*)stmt)->type)) {
                    this->errors.emplace_back(
                        ErrorType::TypeNotFound, ((AstDec*)stmt)->type,
                        "Type does not exist"
                    );
                    return;
                }
            }
        }
    }
}

/*
 * expretion type cheacking
 * flat structs
 * inlineing
 * inline assembly
 *
 * lost exprestion is rteurn type move it into a return ast
 */
void Semantics::pass3(Ast &ast) {
    pass3_nest_att(ast.root);
    pass3_node(ast.root);
}

void Semantics::pass3_nest_att(AstNode *node) {
    if(node->node_type == AstNodeType::AstAttribute) {
        nest_flag = true;
        attributes.push_back((AstAttribute*)node);
    } else if(nest_flag) {
        nest_flag = false;

        for(auto attribute : attributes) {
            node->attributes.push_back(attribute);
        }

        attributes.clear();
    }

    switch(node->node_type) {
    case AstNodeType::AstBlock: {
        auto block = (AstBlock*)node;

        for(auto stmt : block->statements) {
            pass3_nest_att(stmt);
        }

        break;
    }

    case AstNodeType::AstStruct: {
        auto struc = (AstStruct*)node;

        for(auto stmt : struc->block->statements) {
            pass3_nest_att(stmt);
        }

        break;
    }

    case AstNodeType::AstImpl: {
        auto impl = (AstImpl*)node;

        for(auto stmt : impl->block->statements) {
            pass3_nest_att(stmt);
        }

        break;
    }

    default:
        break;
    }
}

void Semantics::pass3_node(AstNode *node) {
    for(auto attribute : node->attributes) {
        if(attribute->name == "il") {
            node->emit = false;
        }
    }

    switch(node->node_type) {
    case AstNodeType::AstBlock: {
        auto block = (AstBlock*)node;

        for(auto stmt : block->statements) {
            pass3_node(stmt);
            stmt = inline_if_need_be(stmt);
        }

        break;
    }

    case AstNodeType::AstString:
        break;

    case AstNodeType::AstNumber:
        break;

    case AstNodeType::AstBoolean:
        break;

    case AstNodeType::AstArray:
        break;

    case AstNodeType::AstDec: {
        auto decl = (AstDec*)node;

        add_local(decl);

        if(!decl->type) {
            decl->type = infer_type(decl->value);
        } /*else {
            if(x->type->name != infer_type(x->value)->name) {
                printf(
                    "you cant assign an \"%s\" to an \"%s\"\n",
                    infer_type(x->value)->name.c_str(),
                    x->type->name.c_str());
            }
        }*/

        if(decl->value) {
            pass3_node(decl->value);
            decl->value = inline_if_need_be(decl->value);
        }

        break;
    }

    case AstNodeType::AstIf: {
        auto if_stmt = (AstIf*)node;

        pass3_node(if_stmt->condition);
        if_stmt->condition = inline_if_need_be(if_stmt->condition);

        for(auto stmt : if_stmt->true_block->statements) {
            pass3_node(stmt);
            stmt = inline_if_need_be(stmt);
        }

        if(if_stmt->false_block) {
            for(auto stmt : if_stmt->false_block->statements) {
                stmt = inline_if_need_be(stmt);
            }
        }

        break;
    }

    case AstNodeType::AstFn: {
        auto fn = (AstFn*)node;

        for(auto func : p2_funcs) {
            if(func != fn && func->mangled_name == fn->mangled_name) {
                this->errors.emplace_back(
                    ErrorType::DuplicateFunctionDeclaration, fn,
                    "Duplicate function declaration"
                );
                return;
            }
        }

        if(fn->body) {
            for(auto stmt : fn->body->statements) {
                pass3_node(fn->body);
            }
        }

        break;
    }

    case AstNodeType::AstFnCall: {
        auto fn_call = (AstFnCall*)node;
        auto fn = p2_get_fn_unmangled(fn_call->name);

        if(!fn_call->mangled && fn && fn->body) {
            fn_call->mangled = true;

            for(auto arg : fn_call->args) {
                fn_call->name += type_to_string(infer_type(arg));
            }
        }

        {
            auto fn = p2_get_fn(fn_call->name);

            if(fn && fn->attributes.empty()) {
                for(auto attribute : fn_call->attributes) {
                    if(attribute->name == "inline") {
                        fn_call->emit = false;
                    }
                }
            }
        }

        {
            auto fn = p2_get_fn(fn_call->name);

            if(fn && fn->body) {
                if(fn->params.size() > fn_call->args.size()) {
                    this->errors.emplace_back(
                        ErrorType::TooManyArguments, fn_call,
                        "Too many arguments to function call"
                    );
                } else if(fn->params.size() < fn_call->args.size()) {
                    this->errors.emplace_back(
                        ErrorType::NotEnoughArguments, fn_call,
                        "Not enough arguments to function call"
                    );
                } else {
                    for(size_t i = 0; i < fn->params.size(); i++) {
                        auto param_type = infer_type(fn->params.at(i));
                        auto arg_type   = infer_type(fn_call->args.at(i));

                        if(param_type->name != arg_type->name) {
                            this->errors.emplace_back(
                                ErrorType::TypeMismatch, param_type,
                                "Type mismatch: expected "s +
                                param_type->name.c_str() + " at argument " +
                                std::to_string(i + 1) + ", got " +
                                arg_type->name.c_str()
                            );
                        }
                    }
                }
            }
        }

        for(auto arg : fn_call->args) {
            arg = inline_if_need_be(arg);
            pass3_node(arg);
        }

        break;
    }

    case AstNodeType::AstLoop: {
        auto loop = (AstLoop*)node;

        pass3_node(loop->expr);

        loop->expr = inline_if_need_be(loop->expr);

        for(auto stmt : loop->body->statements) {
            stmt = inline_if_need_be(stmt);
        }

        break;
    }

    case AstNodeType::AstContinue:
        break;

    case AstNodeType::AstBreak:
        break;

    case AstNodeType::AstStruct:
        break;

    case AstNodeType::AstImpl: {
        auto impl = (AstImpl*)node;

        for(auto stmt : impl->block->statements) {
            pass3_node(stmt);
            stmt = inline_if_need_be(stmt);
        }

        break;
    }

    case AstNodeType::AstAttribute:
        break;

    case AstNodeType::AstAffix: {
        auto affix = (AstAffix*)node;

        p3_affix(affix);

        for(auto stmt : affix->body->statements) {
            pass3_node(stmt);
            stmt = inline_if_need_be(stmt);
        }

        break;
    }

    case AstNodeType::AstUnaryExpr: {
        auto un_expr = (AstUnaryExpr*)node;

        {
            auto fn = p2_get_fn(un_expr->op);

            if(fn && fn->body) {
                for(auto param : fn->params) {
                    un_expr->op += type_to_string(param->type);
                }
            }
        }
        {
            auto fn = p2_get_affix(un_expr->op);

            if(fn && fn->body) {
                for(auto param : fn->params) {
                    un_expr->op += type_to_string(param->type);
                }
            }
        }

        pass3_node(un_expr->expr);
        un_expr->expr = inline_if_need_be(un_expr->expr);

        break;
    }

    case AstNodeType::AstBinaryExpr: {
        auto bin_expr = (AstBinaryExpr*)node;

        {
            auto name = p2_get_fn(bin_expr->op);

            if(name && name->body) {
                for(auto param : name->params) {
                    bin_expr->op += type_to_string(param->type);
                }
            }
        }
        {
            auto name = p2_get_affix(bin_expr->op);

            if(name && name->body) {
                for(auto param : name->params) {
                    bin_expr->op += type_to_string(param->type);
                }
            }
        }

        pass3_node(bin_expr->lhs);
        pass3_node(bin_expr->rhs);

        bin_expr->lhs = inline_if_need_be(bin_expr->lhs);
        bin_expr->rhs = inline_if_need_be(bin_expr->rhs);
        break;
    }

    case AstNodeType::AstIndex:
        break;

    case AstNodeType::AstType:
        break;

    case AstNodeType::AstSymbol:
        break;

    case AstNodeType::AstReturn: {
        auto ret = (AstReturn*)node;
        pass3_node(ret->expr);
        ret->expr = inline_if_need_be(ret->expr);
        break;
    }

    case AstNodeType::AstExtern: {
        auto ext = (AstExtern*)node;

        for(auto decl : ext->decls) {
            pass3_node(decl);
        }

        break;
    }
    }
}

void Semantics::p3_struct(AstStruct *node) {}

/*
 * prefex may onlyhave one arg
 * suffix may only have one arg
 * infix must have 2 args and any return type
 */
void Semantics::p3_affix(AstAffix *node) {
    switch(node->affix_type) {
    case AffixType::Prefix:
        if(node->params.size() > 1) {
            this->errors.emplace_back(
                ErrorType::TooManyArguments, node,
                "Too many arguments: prefix functions can only have one "
                "parameter"
            );
        } else if(node->params.size() < 1) {
            this->errors.emplace_back(
                ErrorType::NotEnoughArguments, node,
                "Not enough arguments: prefix functions must have one "
                "parameter"
            );
        }

        break;

    case AffixType::Suffix:
        if(node->params.size() > 1) {
            this->errors.emplace_back(
                ErrorType::TooManyArguments, node,
                "Too many arguments: suffix functions can only have one "
                "parameter"
            );
        } else if(node->params.size() < 1) {
            this->errors.emplace_back(
                ErrorType::NotEnoughArguments, node,
                "Not enough arguments: suffix functions must have one "
                "parameter"
            );
        }

        break;

    case AffixType::Infix:
        if(node->params.size() > 2) {
            this->errors.emplace_back(
                ErrorType::TooManyArguments, node,
                "Too many arguments: infix functions can only have two "
                "parameters"
            );
        } else if(node->params.size() < 2) {
            this->errors.emplace_back(
                ErrorType::NotEnoughArguments, node,
                "Not enough arguments: infix functions must have two "
                "parameters"
            );
        }

        if(!node->return_type) {
            this->errors.emplace_back(
                ErrorType::NoType, node,
                "Infix functions must have a return type"
            );
        }

        break;
    }
}

AstNode *Semantics::inline_if_need_be(AstNode *node) {
    bool flag = false;

    if(node->attributes.empty()) {
        for(auto attribute : node->attributes) {
            if(attribute->name == "inline") {
                flag = true;
                break;
            }
        }
    }

    if(!flag || node->node_type != AstNodeType::AstFnCall) {
        return node;
    }

    auto fn_call = (AstFnCall*)node;

    return node;
}

AstType *Semantics::infer_type(AstNode *node) {
    if(!node) {
        return nullptr;
    }

    switch(node->node_type) {
    case AstNodeType::AstBlock: {
        auto block = (AstBlock*)node;

        push_scope();

        for(auto stmt : block->statements) {
            auto type = infer_type(stmt);

            if(type) {
                return type;
            }
        }

        pop_scope();

        break;
    }

    case AstNodeType::AstString: {
        auto ret  = new AstType();
        ret->name = "str";
        return ret;
    }

    case AstNodeType::AstNumber: {
        auto number = (AstNumber*)node;
        auto ret    = new AstType();

        if(number->is_float) {
            ret->name = "f" + std::to_string(number->bits);
        } else if(number->is_signed) {
            ret->name = "i" + std::to_string(number->bits);
        } else {
            ret->name = "u" + std::to_string(number->bits);
        }

        return ret;
    }

    case AstNodeType::AstBoolean: {
        auto ret  = new AstType();
        ret->name = "bool";
        return ret;
    }

    case AstNodeType::AstArray: {
        //auto x = (AstArray *)node;
        // fml
        break;
    }

    case AstNodeType::AstDec: {
        auto decl = (AstDec*)node;
        add_local(decl);
        return clone_type(decl->type);
    }

    case AstNodeType::AstIf:
        this->errors.emplace_back(
            ErrorType::CompilerError, node,
            "Attempt to infer the type of an if statement"
        );
        break;

    case AstNodeType::AstFn: {
        return clone_type(((AstFn*)node)->return_type);
    }

    case AstNodeType::AstFnCall: {
        auto fn_call = (AstFnCall*)node;

        {
            auto type = infer_type(p2_get_fn(fn_call->name));

            if(type) {
                return type;
            }
        }
        {
            auto type = infer_type(p2_get_affix(fn_call->name));

            if(type) {
                return type;
            }
        }

        break;
    }

    case AstNodeType::AstLoop:
        this->errors.emplace_back(
            ErrorType::CompilerError, node,
            "Attempt to infer the type of a loop statement"
        );
        break;

    case AstNodeType::AstContinue:
        this->errors.emplace_back(
            ErrorType::CompilerError, node,
            "Attempt to infer the type of a continue statement"
        );
        break;

    case AstNodeType::AstBreak:
        this->errors.emplace_back(
            ErrorType::CompilerError, node,
            "Attempt to infer the type of a break statement"
        );
        break;

    case AstNodeType::AstStruct: {
        auto ret  = new AstType();
        ret->name = ((AstStruct*)node)->name;
        return ret;
    }

    case AstNodeType::AstImpl:
        this->errors.emplace_back(
            ErrorType::CompilerError, node,
            "Attempt to infer the type of an impl statement"
        );
        break;

    case AstNodeType::AstAttribute:
        this->errors.emplace_back(
            ErrorType::CompilerError, node,
            "Attempt to infer the type of an attribute"
        );
        break;

    case AstNodeType::AstAffix:
        return clone_type(((AstAffix*)node)->return_type);

    case AstNodeType::AstUnaryExpr: {
        auto un_expr = (AstUnaryExpr*)node;

        {
            auto type = infer_type(p2_get_fn(un_expr->op));

            if(type) {
                return type;
            }
        }
        {
            auto type = infer_type(p2_get_affix(un_expr->op));

            if(type) {
                return type;
            }
        }

        break;
    }

    case AstNodeType::AstBinaryExpr: {
        auto bin_expr = (AstBinaryExpr*)node;

        {
            auto type = infer_type(p2_get_fn(bin_expr->op));

            if(type) {
                return type;
            }
        }
        {
            auto type = infer_type(p2_get_affix(bin_expr->op));

            if(type) {
                return type;
            }
        }

        break;
    }

    case AstNodeType::AstIndex:
        return infer_type(((AstIndex*)node)->expr);

    case AstNodeType::AstType:
        return clone_type((AstType*)node);

    case AstNodeType::AstSymbol: {
        auto symbol = (AstSymbol*)node;

        {
            auto fn = p2_get_fn(symbol->name);

            if(fn) {
                return clone_type(fn->return_type);
            }
        }
        {
            auto fn = p2_get_affix(symbol->name);

            if(fn) {
                return clone_type(fn->return_type);
            }
        }
        {
            auto local = get_local(symbol->name);

            if(local) {
                return infer_type(local->value);
            }
        }

        break;
    }

    case AstNodeType::AstReturn:
        this->errors.emplace_back(
            ErrorType::CompilerError, node,
            "Attempt to infer the type of a return statement"
        );
        break;

    case AstNodeType::AstExtern:
        this->errors.emplace_back(
            ErrorType::CompilerError, node,
            "Attempt to infer the type of an extern statement"
        );
        break;
    }

    return nullptr;
}
