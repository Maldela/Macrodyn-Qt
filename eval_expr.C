///////////////////////////////////////////////////////////////////////////////
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/RCS/eval_expr.C,v 1.1 2000/09/15 10:14:44 mhoffman Exp $
//
// Module name:		eval_expr.C
// Contents:		evaluate an expression which contains
//			{constant | name } [ { + | - | * | /} {constant | name}]
//
// Author:		Uli Middelberg
// Last modified:	Thu Oct  2 14:53:47 1997
// By:			Uli Middelberg
// 
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "get_expr.h"
#include "eval_expr.h"

qreal eval_expr
  ( baseModel  * model, 	// to resolve names
    const char * expr )		// the expression to be evaluated
{   qreal       * value  = NULL;
    const char * pos    = expr;
    char       * foo    = strdup(expr);
    qreal         result = 0;
    qreal         val1   = 0;
    qreal         val2   = 0;

  if ( (pos = strpbrk(expr,"+-*/")) == NULL ) {	// this is an expression
    strcpy(foo, expr);
    if ( (value=model->setLabels(foo)) != NULL ) {	// without +-*/
      result = *value;
    } else {
      result = atof(expr);
    }
  } else {
    char * token  = strdup(expr);
    char   op     = *pos;				// operator

    pos = get_expr(expr,token,op);
    							// token contains		
    if ( (value=model->setLabels(token)) != NULL ) {	// 1st operand
      val1 = *value;
    } else {
      val1=atof(token);
    }
    strcpy(foo, pos);					// pos points to the
    if ( (value=model->setLabels(foo)) != NULL ) {	// 2nd operand
      val2 = *value;
    } else {
      val2=atof(foo);
    }
    
    switch (op) {
      case '+' : result = val1 + val2; break;
      case '-' : result = val1 - val2; break;
      case '*' : result = val1 * val2; break;
      case '/' : result = val1 / val2; break;
      default  : error("eval_expr: unknown operand: %c", QChar(op));
    }
    free(token);
  }
  return(result);
}


// eof
