#ifndef ISEMANTICANALYSIS_H
#define ISEMANTICANALYSIS_H

#include <vector>
#include <stdint.h>
#include "AstDefs.h"
#include "ScopeContext.h"
#include "DuskAssembly.h"

class ISemanticAnalysis {
public:
    virtual bool validate_types(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node) = 0;
    virtual bool validate_semantics(
        DuskAssembly &ds, ScopeContext *scope, AstNode *node) = 0;

    AstNodeType type_handler;
    uint32_t pass;

    static std::vector<ISemanticAnalysis *> handlers;
};



#endif /* ISEMANTICANALYSIS_H */
