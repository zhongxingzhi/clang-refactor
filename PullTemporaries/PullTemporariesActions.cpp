//===-- PullTemporaries/PullTemporariesActions.cpp - Matcher callback ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains the definition of the PullTemporariesFixer class which is
/// used as an ASTMatcher callback. Also within this file is a helper AST
/// visitor class used to identify sequences of explicit casts.
///
//===----------------------------------------------------------------------===//

#include "PullTemporariesActions.h"
#include "PullTemporariesMatchers.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "clang/Basic/CharInfo.h"
#include "clang/Lex/Lexer.h"

#include "Core/Utility.h"

#include <iostream>

using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace clang;

using namespace TransformationUtility;

namespace {
void ReplaceWith(Transform &Owner, SourceManager &SM,
                        SourceLocation StartLoc, SourceLocation EndLoc, const clang::ASTContext& Context, const Expr* argument ){ 
    using namespace std;

  CharSourceRange Range(SourceRange(StartLoc, EndLoc), true);

  string source_text = getString( argument, SM );
  string replacement = source_text; 

  if ( isReplaceableRange( StartLoc, EndLoc, SM, Owner ) ){ 
      Owner.addReplacementForCurrentTU( tooling::Replacement(SM, Range, replacement ));
  }
}
void ReplaceWith(Transform &Owner, SourceManager &SM,
                        SourceLocation StartLoc, SourceLocation EndLoc, const clang::ASTContext& Context, std::string replacement ){ 
    using namespace std;

  CharSourceRange Range(SourceRange(StartLoc, EndLoc), true);

  if ( isReplaceableRange( StartLoc, EndLoc, SM, Owner ) ){ 
      Owner.addReplacementForCurrentTU( tooling::Replacement(SM, Range, replacement ));
  }
}
}

PullTemporariesFixer::PullTemporariesFixer(unsigned &AcceptedChanges,
                           Transform &Owner)
    : AcceptedChanges(AcceptedChanges), Owner(Owner) {
}


void PullTemporariesFixer::run(const ast_matchers::MatchFinder::MatchResult &Result) {
  using namespace std;
  ASTContext& context = *Result.Context;
  SourceManager& SM = context.getSourceManager();
  //static list<Decl*> removed_declares;

  const auto* node = Result.Nodes.getNodeAs<DeclRefExpr>(MatcherPullTemporariesID);
  if ( node ) {
      if ( !Owner.isInRange( node, SM ) ) return;
      auto* value_decl = node->getDecl();
      auto name = value_decl->getNameAsString();
      auto it_found = name.find("temp_");
      if ( it_found != string::npos ) {
	  // thats ok for now 
	  // TODO put this into the matcher
	  auto* var_decl = dyn_cast_or_null<VarDecl>(value_decl);
	  if ( !var_decl ) return;
	  // get its initializer
	  // TODO put this into the matcher
	  if ( !var_decl->hasInit() ) return;
	  auto* init = var_decl->getInit();
	  {
	      SourceLocation StartLoc = node->getLocStart();
	      SourceLocation EndLoc = node->getLocEnd();
	      ReplaceWith( Owner, SM, StartLoc, EndLoc, context, init );
	  }

	  {
	      SourceLocation StartLoc = var_decl->getLocStart();
	      SourceLocation EndLoc = var_decl->getLocEnd();
	      ReplaceWith( Owner, SM,StartLoc, EndLoc, context, "" );
	  }
      }

  }
  // TODO replace the declare with nothing 
  

}














