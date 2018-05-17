#ifndef ICODEGENERATOR_H
#define ICODEGENERATOR_H

#include <vector>
#include <stdint.h>
#include "AstDefs.h"
#include "ScopeContext.h"
#include "DuskAssembly.h"

class ICodeGenerator {
public:
    virtual void generate(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node,
        ILBinary &binary) = 0;

    AstNodeType type_handler;

    static std::vector<ICodeGenerator> handlers;
};

#endif /* ICODEGENERATOR_H */
