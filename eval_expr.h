 // $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/RCS/eval_expr.h,v 1.1 2000/09/15 10:14:48 mhoffman Exp $

#ifndef _EVAL_EXPR_INCLUDED
#define _EVAL_EXPR_INCLUDED

#include "sim.h"
#include "Models/baseModel.h"

extern qreal eval_expr
    ( baseModel  * model, 	// to resolve names
      const char * expr )	// the expression to be evaluated
;

#endif // _EVAL_EXPR_INCLUDED

// eof
