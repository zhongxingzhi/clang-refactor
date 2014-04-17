//===-- ForLoopStartFromZero/ForLoopStartFromZeroMatchers.cpp - Matchers for null casts ----------===//
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

#include "ForLoopStartFromZeroMatchers.h"
#include "clang/AST/ASTContext.h"

using namespace clang::ast_matchers;
using namespace clang;

const char *MatcherForLoopStartFromZeroID = "matcherForLoopStartFromZeroID";
const char *MatcherForLoopVariableID = "matcherForLoopVariableID";

// TODO has to have a single init statement 
// TODO has to have a single init statement with an assign initializer with a literal integer 
// TODO has to have a test condition with <= 
// TODO all references to the loop variable have to be inside a minus operator 
//      that subtracts the literal integer on the rhs 

StatementMatcher makeForLoopStartFromZeroMatcher(){
    return forStmt(
	   hasLoopInit(
	       anyOf(
		   declStmt(
		       hasSingleDecl(
			   varDecl(
			       hasInitializer(
				   ignoringParenImpCasts(
					integerLiteral().bind("init")
				   )
			       )
			   ).bind(MatcherForLoopVariableID)
		       )
		   ),
		   binaryOperator(
		       hasOperatorName("="),
		       hasLHS(
			   declRefExpr(
			       to(
				    varDecl().bind(MatcherForLoopVariableID)
				)
			   )
		       ),
		       hasRHS(
			    ignoringParenImpCasts(
				integerLiteral().bind("init")
			    )
		       )
		   )
	       )
	   ),
	   hasCondition(
		binaryOperator(
		    hasOperatorName("<=")
		)
	   )
#if 1
	   ,
	   hasBody(
		forEachDescendant(
		    declRefExpr(
			to(
			    decl(
#if 1
				equalsBoundNode(MatcherForLoopVariableID),
				hasParent(
				    binaryOperator(
					hasOperatorName("-"),
					hasRHS(
					    ignoringParenImpCasts(
						integerLiteral(
						    equalsBoundNode("init")
						)
					    )
					)
				    )
				)
#endif
			    )
			)
		     )
		)
	   )
#endif
    ).bind(MatcherForLoopStartFromZeroID);
}












