#include "ScopeContext.h"

void ScopeContext::import_scope(ScopeContext *scope) {

}

void ScopeContext::enter(AstNode *node) {}
void ScopeContext::leave(AstNode *node) {}

bool ScopeContext::func_exists(std::string name,
                               std::vector<AstType *> param_types) {}
AstFn *ScopeContext::func_get(std::string name,
                              std::vector<AstType *> param_types) {}
void ScopeContext::func_add(AstFn *fn) {}

bool ScopeContext::affix_exists(std::string name,
                                std::vector<AstType *> param_types) {}
AstAffix *ScopeContext::affix_get(std::string name,
                                  std::vector<AstType *> param_types) {}
void ScopeContext::affix_add(AstAffix *affix) {}

bool ScopeContext::struct_exists(std::string name) {}
AstStruct *ScopeContext::struct_get(std::string name) {}
void ScopeContext::struct_add(AstStruct *struc) {}

bool ScopeContext::global_exists(std::string name) {}
AstDec *ScopeContext::global_get(std::string name) {}
void ScopeContext::global_add(AstDec *dec) {}

bool ScopeContext::arg_exists(std::string name) {}
AstDec *ScopeContext::arg_get(std::string name) {}
void ScopeContext::arg_add(AstDec *arg) {}

bool ScopeContext::local_exists(std::string name) {}
AstDec *ScopeContext::local_get(std::string name) {}
void ScopeContext::local_add(AstDec *local) {}