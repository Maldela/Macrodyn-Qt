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

#include <stdlib.h>
#include <string.h>

#include "../error.h"
#include "../strnchr.h"
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
      const char * gen, 	// the basic random generator
      const char * states, 	// definition of the states and their qrealisations
      const char * matrix )	// the transition matrix 
{
    const char * pos    = states;
    char       * token  = strdup(states);
    int          n;

  b_model = model;
  r_gen = strdup(gen);

  n_states = strnchr(states, ';');	// get the numbet of states
  qrealization = new double [n_states];
  trans = new rand_var * [n_states];
  state = 0;
  
  pos = get_expr(pos,token,'{');	// skip the leading {

  for( n=0; n < n_states && (pos = get_expr(pos,token,';')) ; n ++ ) {
    qrealization[n] = eval_expr(b_model, token);
    trans[n] = NULL;
  }

  free(token);

  token = strdup(matrix);
  pos = matrix;
  
  for( n=0; n < n_states && (pos = get_expr(pos,token," ")) ; n ++ ) {
    set_row(n, token);
  }
  
  free(token);
  
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
  free(r_gen);
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
void markov_chain::set_row ( int row, const char * expr)
{   const char * pos    = expr;
    char * token = strdup(expr);
    
    char * zvar_expr = new char [1024];
    char * buf = new char [1024];

  if( row < 0 || row >=n_states ) {
    error("markov_chain::set_row: value for row %d is not in 0..%d", QString::number(row) += QString::number(n_states));
  }

  zvar_expr[0]='\0';
  
  for( int n = 0; n < n_states && (pos = get_expr(pos,token,';')); n++) {
    sprintf(buf,"%s[%d,%d];",token, n, n);
    strcat(zvar_expr,buf);
  }

  if( trans[row] != NULL ) {
    delete trans[row];
  }
  
  trans[row] = new rand_var(b_model, r_gen, zvar_expr);

  free(token);
  delete [] zvar_expr;
  delete [] buf;
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
