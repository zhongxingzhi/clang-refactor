//===-- RenameVariable/RenameVariableMatchers.cpp - Matchers for null casts ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains the definitions for matcher-generating functions
/// and a custom AST_MATCHER for identifying casts of type CK_NullTo*.
///
//===----------------------------------------------------------------------===//

#include "RenameVariableMatchers.h"
#include "clang/AST/ASTContext.h"

using namespace clang::ast_matchers;
using namespace clang;

const char *MatcherRenameVariableID = "matcherRenameVariableID";


StatementMatcher makeRenameVariableMatcher(){
    return declRefExpr().bind(MatcherRenameVariableID);
}

#if 0
StatementMatcher makeRenameVariableMatcherDecl(){
    return declStmt().bind(MatcherRenameVariableDeclID);
}
#endif










