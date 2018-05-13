#include "CodeGen.h"

void generate_il(AstNode *node, ILemitter &il, Semantics &sem) {
    if(!node) {
        return;
    }

    if(!node->emit) {
        return;
    }

    node->code_gen(il, sem);
}
