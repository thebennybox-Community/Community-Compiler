#ifndef ISEMANTICGENERATOR_H
#define ISEMANTICGENERATOR_H

#include <vector>
#include <stdint.h>
#include "AstDefs.h"
#include "ScopeContext.h"
#include "DuskAssembly.h"

class ICodeGenerator {
public:
    virtual AstNode* generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node) = 0;

    AstNodeType type_handler;
    uint32_t pass;

    static std::vector<ISemanticGenerator> handlers;
};

#endif /* ISEMANTICGENERATOR_H */
