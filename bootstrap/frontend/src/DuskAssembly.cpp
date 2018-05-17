#include "DuskAssembly.h"

#include <fstream>
#include "TokenStream.h"
#include "Parser.h"
#include "Terminal.h"
#include "AstPrettyPrinter.h"
#include "ISemanticAnalysis.h"
#include "ISemanticGenerator.h"
#include "ICodeGenerator.h"
#include "SemanticGenerator.cpp"
#include "SemanticsAnalysis.cpp"
#include "CodeGen.cpp"

std::vector<ISemanticGenerator *> ISemanticGenerator::handlers;
std::vector<ICodeGenerator *> ICodeGenerator::handlers;
std::vector<ISemanticAnalysis *> ISemanticAnalysis::handlers;

DuskAssembly::DuskAssembly() {
    ISemanticAnalysis::handlers.push_back(new AstBlockAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstStringAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstNumberAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstBooleanAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstArrayAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstDecAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstIfAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstFnAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstFnCallAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstLoopAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstContinueAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstBreakAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstStructAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstImplAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstAttributeAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstAffixAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstUnaryExprAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstBinaryExprAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstIndexAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstTypeAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstSymbolAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstReturnAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstExternAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstUseAnalysis());
    ISemanticAnalysis::handlers.push_back(new AstNamespaceAnalysis());


    ISemanticGenerator::handlers.push_back(new AstBlockGenerator());
    ISemanticGenerator::handlers.push_back(new AstStringGenerator());
    ISemanticGenerator::handlers.push_back(new AstNumberGenerator());
    ISemanticGenerator::handlers.push_back(new AstBooleanGenerator());
    ISemanticGenerator::handlers.push_back(new AstArrayGenerator());
    ISemanticGenerator::handlers.push_back(new AstDecGenerator());
    ISemanticGenerator::handlers.push_back(new AstIfGenerator());
    ISemanticGenerator::handlers.push_back(new AstFnGenerator());
    ISemanticGenerator::handlers.push_back(new AstFnCallGenerator());
    ISemanticGenerator::handlers.push_back(new AstLoopGenerator());
    ISemanticGenerator::handlers.push_back(new AstContinueGenerator());
    ISemanticGenerator::handlers.push_back(new AstBreakGenerator());
    ISemanticGenerator::handlers.push_back(new AstStructGenerator());
    ISemanticGenerator::handlers.push_back(new AstImplGenerator());
    ISemanticGenerator::handlers.push_back(new AstAttributeGenerator());
    ISemanticGenerator::handlers.push_back(new AstAffixGenerator());
    ISemanticGenerator::handlers.push_back(new AstUnaryExprGenerator());
    ISemanticGenerator::handlers.push_back(new AstBinaryExprGenerator());
    ISemanticGenerator::handlers.push_back(new AstIndexGenerator());
    ISemanticGenerator::handlers.push_back(new AstTypeGenerator());
    ISemanticGenerator::handlers.push_back(new AstSymbolGenerator());
    ISemanticGenerator::handlers.push_back(new AstReturnGenerator());
    ISemanticGenerator::handlers.push_back(new AstExternGenerator());
    ISemanticGenerator::handlers.push_back(new AstUseGenerator());
    ISemanticGenerator::handlers.push_back(new AstNamespaceGenerator());

    ICodeGenerator::handlers.push_back(new AstBlockCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstStringCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstNumberCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstBooleanCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstArrayCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstDecCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstIfCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstFnCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstFnCallCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstLoopCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstContinueCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstBreakCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstStructCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstImplCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstAttributeCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstAffixCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstUnaryExprCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstBinaryExprCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstIndexCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstTypeCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstSymbolCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstReturnCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstExternCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstUseCodeGenerator());
    ICodeGenerator::handlers.push_back(new AstNamespaceCodeGenerator());
}


static int find_total_passes() {
    int ret = 0;

    for(auto &handler : ISemanticAnalysis::handlers) {
        if(handler->pass > ret) {
            ret = handler->pass;
        }
    }

    for(auto &handler : ISemanticGenerator::handlers) {
        if(handler->pass > ret) {
            ret = handler->pass;
        }
    }

    return ret;
}

static std::string read_file(std::string filename) {
    std::ifstream stream(filename);
    std::string str(
        (std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>());
    return str;
}

ScopeContext *DuskAssembly::find_scope(AstNamespace *ns) {
    for(auto scope : this->scopes) {
        if(scope->ns->name == ns->name) {
            return scope;
        }
    }

    return nullptr;
}

void DuskAssembly::queue_file(std::string filename) {
    this->queued_files.push_back(filename);
}


void DuskAssembly::compile_write_binary(std::string out_file) {

    std::vector<Ast> asts;

    for(auto file : queued_files) {
        asts.push_back(parse_file(file));
    }

    for(int pass = 0; pass < find_total_passes(); pass++) {
        for(auto ast : asts) {
            semantic_generation(ast, pass);
            semantic_analysis(ast, pass);
        }
    }

    for(auto ast : asts) {
        generate_code(ast);
    }
}

Ast DuskAssembly::parse_file(std::string filename) {
    Ast ast;
    ast.file_contents = read_file(filename);

    TokenStream token_stream;
    token_stream.lex(ast.file_contents);
    ast.tokens = token_stream.tokens;

    if(handle_errors(token_stream.errors, ast.file_contents, ast.tokens)) {
        Parser parser;
        Ast tmp = parser.parse(token_stream.tokens);

        // Discard result of first pass
        delete tmp.root;

        if(handle_errors(parser.errors, ast.file_contents, ast.tokens)) {
            Ast tmp = parser.parse(token_stream.tokens);
            ast.root = tmp.root;
            ast.tokens = token_stream.tokens;
        }
    }

    return ast;
}


bool DuskAssembly::semantic_generation(Ast &ast, int pass) {
    return semantic_generation_node(ast.root, pass);
}


bool DuskAssembly::semantic_generation_node(AstNode *node, int pass) {

    for(auto handler : ISemanticGenerator::handlers) {
        if(handler->type_handler == node->node_type) {
            handler->pass = pass;
            handler->generate(*this, scopes.front(), node);
            break;
        }
    }

    switch(node->node_type) {
    case AstNodeType::AstBlock: {
        AstBlock *block = (AstBlock *) node;

        for(auto stmt : block->statements) {
            semantic_generation_node(stmt, pass);
        }
    }
    break;

    case AstNodeType::AstIf: {
        auto x = (AstIf *) node;
        semantic_generation_node(x->true_block, pass);
        semantic_generation_node(x->false_block, pass);
    }

    break;

    case AstNodeType::AstFn: {
        auto x = (AstFn *) node;
        semantic_generation_node(x->body, pass);
    }
    break;

    case AstNodeType::AstLoop: {
        auto x = (AstLoop *) node;
        semantic_generation_node(x->body, pass);
    }
    break;

    case AstNodeType::AstImpl: {
        auto x = (AstImpl *) node;
        semantic_generation_node(x->block, pass);
    }
    break;


    case AstNodeType::AstAffix: {
        auto x = (AstAffix *) node;
        semantic_generation_node(x->body, pass);
    }
    break;

    case AstNodeType::AstExtern: {
        auto x = (AstExtern *) node;

        for(auto stmt : x->decls) {
            semantic_generation_node(stmt, pass);
        }
    }
    break;

    }
}


bool DuskAssembly::semantic_analysis(Ast &ast, int pass) {
    return semantic_analyse_node(ast.root, pass);
}

bool DuskAssembly::semantic_analyse_node(AstNode *node, int pass) {
    for(auto handler : ISemanticAnalysis::handlers) {
        if(handler->type_handler == node->node_type) {
            handler->pass = pass;
            handler->validate_semantics(*this, scopes.front(), node);
            handler->validate_types(*this, scopes.front(), node);
            break;
        }
    }

    switch(node->node_type) {
    case AstNodeType::AstBlock: {
        AstBlock *block = (AstBlock *) node;

        for(auto stmt : block->statements) {
            semantic_analyse_node(stmt, pass);
        }
    }
    break;

    case AstNodeType::AstIf: {
        auto x = (AstIf *) node;
        semantic_analyse_node(x->true_block, pass);
        semantic_analyse_node(x->false_block, pass);
    }

    break;

    case AstNodeType::AstFn: {
        auto x = (AstFn *) node;
        semantic_analyse_node(x->body, pass);
    }
    break;

    case AstNodeType::AstLoop: {
        auto x = (AstLoop *) node;
        semantic_analyse_node(x->body, pass);
    }
    break;

    case AstNodeType::AstImpl: {
        auto x = (AstImpl *) node;
        semantic_analyse_node(x->block, pass);
    }
    break;


    case AstNodeType::AstAffix: {
        auto x = (AstAffix *) node;
        semantic_analyse_node(x->body, pass);
    }
    break;

    case AstNodeType::AstExtern: {
        auto x = (AstExtern *) node;

        for(auto stmt : x->decls) {
            semantic_analyse_node(stmt, pass);
        }
    }
    break;

    }
}


bool DuskAssembly::generate_code(Ast &ast) {
    return generate_code_node(ast.root);
}

bool DuskAssembly::generate_code_node(AstNode *node) {

    for(auto handler : ICodeGenerator::handlers) {
        if(handler->type_handler == node->node_type) {
            handler->generate(*this, scopes.front(), node, il_emitter);
            break;
        }
    }

    switch(node->node_type) {
    case AstNodeType::AstBlock: {
        AstBlock *block = (AstBlock *) node;

        for(auto stmt : block->statements) {
            generate_code_node(stmt);
        }
    }
    break;

    case AstNodeType::AstIf: {
        auto x = (AstIf *) node;
        generate_code_node(x->true_block);
        generate_code_node(x->false_block);
    }

    break;

    case AstNodeType::AstFn: {
        auto x = (AstFn *) node;
        generate_code_node(x->body);
    }
    break;

    case AstNodeType::AstLoop: {
        auto x = (AstLoop *) node;
        generate_code_node(x->body);
    }
    break;

    case AstNodeType::AstImpl: {
        auto x = (AstImpl *) node;
        generate_code_node(x->block);
    }
    break;


    case AstNodeType::AstAffix: {
        auto x = (AstAffix *) node;
        generate_code_node(x->body);
    }
    break;

    case AstNodeType::AstExtern: {
        auto x = (AstExtern *) node;

        for(auto stmt : x->decls) {
            generate_code_node(stmt);
        }
    }
    break;

    }
}


bool DuskAssembly::handle_errors(
    std::vector<Error> errors,
    std::string file_contents, std::vector<Token> tokens
) {
    for(Error error : errors) {
        printf(
            "\n%s%s @ %s%s%d%s:%s%d%s\n",
            term_fg[TermColour::Yellow],
            error.message.c_str(),
            term_reset,
            term_fg[TermColour::Blue], error.line, term_reset,
            term_fg[TermColour::Blue], error.column, term_reset
        );
        syntax_highlight_print_error(
            file_contents, tokens,
            error.line, error.offset, error.count);
    }

    return errors.empty();
}

