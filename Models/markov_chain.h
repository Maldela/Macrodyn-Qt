///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/markov_chain.h,v 1.1 2000/08/31 16:00:58 mmeyer Exp $
//
// Module name:		markov_chain.h
// Contents:		markov chain generator for macrodyn
//
// Author:		Uli Middelberg
// Last modified:	Wed Jul  9 12:16:55 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _MARKOV_CHAIN_INCLUDED
#define _MARKOV_CHAIN_INCLUDED

#include "baseModel.h"

#include "rand_var.h"

class markov_chain
{
protected:
    int          n_states;	// number of states of the process
    int          state;		// the current state
    double     * qrealization;	// the qrealization for each state
    rand_var * * trans;		// transition matrix
    baseModel  * b_model;	// to resolve names
    QString      r_gen;		//
    
    void set_row ( int row,		// set the probabilities in this row
                  QString expr);	// according to the expression
    
public:
    markov_chain			// constructor 
             ( baseModel* model, 	// to resolve names
               const QString& gen, 	// the basic random generator
               QString states,	// definition of the states and their qrealisations
               QString matrix);    // the transition matrix
               				
    ~markov_chain();		//  distructor
 
    double dice();			// returns the next dice
};


#endif	// _MARKOV_CHAIN_INCLUDED
// eof
