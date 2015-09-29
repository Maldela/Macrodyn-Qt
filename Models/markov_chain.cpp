///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/markov_chain.C,v 1.1 2000/08/31 16:00:49 mmeyer Exp $
//
// Module name:		markov_chain.C
// Contents:		markov chain generator for macrodyn
//
// Author:		Uli Middelberg
// Last modified:	Wed Jul  9 12:33:15 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#include "../logger.h"

#include "../eval_expr.h"
#include "../get_expr.h"

#include "baseModel.h"

#include "markov_chain.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		markov_chain
// Member function:	markov_chain
// Purpose:		constructor 
//
// Author:		Uli Middelberg
// Last modified:	Wed Jul  9 12:33:18 1997
// By:			Uli Middelberg
//
// Usage:		markov_chain(model,"ranf","{2.3;1.0;4.5;}","0.3;0.3;0.4; 0.3;0.3;0.4; 0.3;0.3;0.4; ")
//
///////////////////////////////////////////////////////////////////////////////

markov_chain::markov_chain
    ( baseModel  * model, 	// to resolve names
      const QString& gen, 	// the basic random generator
      QString states, 	// definition of the states and their qrealisations
      QString matrix )	// the transition matrix
{
    QString token;
    int          n;

  b_model = model;
  r_gen = gen;

  n_states = states.count(';');	// get the numbet of states
  qrealization = new double [n_states];
  trans = new rand_var * [n_states];
  state = 0;
  
  states = get_expr(states, token, QRegExp("{"));	// skip the leading {

  for (n=0; n < n_states && !(states = get_expr(states, token, QRegExp(";"))).isEmpty() ; n ++)
  {
      qrealization[n] = eval_expr(b_model, token);
      trans[n] = NULL;
  }
  
  for (n=0; n < n_states && !(matrix = get_expr(matrix, token, QRegExp(" "))).isEmpty() ; n ++)
  {
      set_row(n, token);
  }  
}

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		markov_chain
// Member function:	~markov_chain
// Purpose:		distructor 
//
// Author:		Uli Middelberg
// Last modified:	Wed Jul  9 12:33:18 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

markov_chain::~markov_chain ( void )
{
  for( int n=0; n < n_states; n ++ ) {
    delete trans[n];
  }
  delete [] qrealization;
  delete [] trans;
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		markov_chain
// Member function:	set row
// Purpose:		sets the probabilities in this row according to the expression
//			i.e put a new random variable in trans[row]
//
// Author:		Uli Middelberg
// Last modified:	Wed Jul  9 12:33:18 1997
// By:			Uli Middelberg
//
// Usage:		set_row("0.3;0.4;0.3;")
//
///////////////////////////////////////////////////////////////////////////////
void markov_chain::set_row(int row, QString expr)
{
    QString token;
    
    QString zvar_expr;

    if (row < 0 || row >= n_states)
        error("markov_chain::set_row: value for row %d is not in 0..%d", QString::number(row) += QString::number(n_states));
  
    for (int n = 0; n < n_states && !(expr = get_expr(expr, token, QRegExp(";"))).isEmpty(); n++)
    {
        const char *pointer = token.toLatin1().data();
        zvar_expr.sprintf("%s[%d,%d];", pointer, n, n);
    }

    if( trans[row] != NULL )
        delete trans[row];
  
    trans[row] = new rand_var(b_model, r_gen, zvar_expr);
}
    
///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		markov_chain
// Member function:	dice
// Purpose:		computes the next qrealisation of the markov chain
//
// Author:		Uli Middelberg
// Last modified:	Wed Jul  9 12:33:18 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

double markov_chain::dice ( void )
{   
  state = (int) trans[state]->dice();	// set the new state by taking the dice
  					// of the corresponding random variable
  					
  return qrealization[state];		// return the qrealization

}

// eof
