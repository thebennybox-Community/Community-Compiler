#ifndef SRC_CODEGEN_H
#define SRC_CODEGEN_H

#include "Ast.h"

class CodeGen {
  public:
    void generateIL(AstNode *node);
};

#endif //SRC_CODEGEN_H
