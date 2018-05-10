#include "Ast.h"

Ast::~Ast() {
    delete root;
}

void AstBlock::code_gen(ILemitter &il, Semantics &sem) {

}

void AstString::code_gen(ILemitter &il, Semantics &sem) {

}

void AstNumber::code_gen(ILemitter &il, Semantics &sem) {

}

void AstBoolean::code_gen(ILemitter &il, Semantics &sem) {

}

void AstArray::code_gen(ILemitter &il, Semantics &sem) {

}

void AstDec::code_gen(ILemitter &il, Semantics &sem) {

}

void AstIf::code_gen(ILemitter &il, Semantics &sem) {

}

void AstFn::code_gen(ILemitter &il, Semantics &sem) {

}

void AstFnCall::code_gen(ILemitter &il, Semantics &sem) {

}

void AstLoop::code_gen(ILemitter &il, Semantics &sem) {

}

void AstContinue::code_gen(ILemitter &il, Semantics &sem) {

}

void AstBreak::code_gen(ILemitter &il, Semantics &sem) {

}

void AstStruct::code_gen(ILemitter &il, Semantics &sem) {

}

void AstImpl::code_gen(ILemitter &il, Semantics &sem) {

}

void AstAttribute::code_gen(ILemitter &il, Semantics &sem) {

}

void AstAffix::code_gen(ILemitter &il, Semantics &sem) {

}

void AstUnaryExpr::code_gen(ILemitter &il, Semantics &sem) {

}

void AstBinaryExpr::code_gen(ILemitter &il, Semantics &sem) {

}

void AstIndex::code_gen(ILemitter &il, Semantics &sem) {

}

void AstType::code_gen(ILemitter &il, Semantics &sem) {

}

void AstSymbol::code_gen(ILemitter &il, Semantics &sem) {

}

void AstReturn::code_gen(ILemitter &il, Semantics &sem) {

}

void AstExtern::code_gen(ILemitter &il, Semantics &sem) {

}
