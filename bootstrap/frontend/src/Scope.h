#ifndef SCOPE_H
#define SCOPE_H

#include <stdint.h>
#include <vector>
#include "AstDefs.h"

struct Scope {
    uint32_t id;
    AstNode *owner;
    std::string owner_name;
    std::string entry_label;
    std::string exit_label;
    std::vector<AstDec *> args;
    std::vector<AstDec *> locals;
};

#endif /* SCOPE_H */
