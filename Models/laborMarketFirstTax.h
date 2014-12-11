#ifndef LABORMARKETFIRST_TAX_H
#define LABORMARKETFIRST_TAX_H

#include "laborMarketFirst.h"
#include "rand_var.h" 

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/laborMarketFirstTax.h,v 1.1 2000/08/31 15:58:30 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  laborMarketFirstTax.h                                        */
/* Contents:     Class definition of the class laborMarketFirstTax            */
/*               This model is derived from the Model labormarketfirst.       */
/*               The difference is that taxS and taxW are set by tax          */
/*                                                                            */
/* Last Modified: 06.03.1991 (Marc Mueller)                                   */
/* Modified:      18.03.1991 (Marc Mueller)                                   */
/*                                                                            */
/******************************************************************************/

class laborMarketFirstTax : public laborMarketFirst {
protected:
    qreal tax;
    void setTax();			// set taxS,taxW by tax
public:
    void loadParamset(QTextStream&);	// load parameterset from a file
    void saveParamset(QTextStream&);	// write parameterset into a file
    void saveParamsetWithNames(QTextStream&); // add parametset to printerfile (so far only for xpm)
    void printParamset();		// print parameterset on the screen
    void sendParameters(int&,qreal**);   // write all parameters into an array
                                        // and return the numbers of parameters
    void receiveParameters(const QList<qreal>&);// receive parameter values

    qreal* setLabels(const QString&);		 // return a pointer to a variable or
    laborMarketFirstTax();		// constructor
    void iteration(const qint64&);	// perform one iteration
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rlaborMarketFirstTax
// Derived from:        laborMarketFirstTax
// Purpose:             defines the laborMarketFirstTax with random variables
//
// Author:              Uli Middelberg
// Last modified:       Fri Jul 25 16:52:34 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class rlaborMarketFirstTax : public laborMarketFirstTax {
private:
    QString zvar_expr;        // expression which defines a random variable
    rand_var * zvar;            // a random variable    
    QString zvar_name;        // name of the stochastified parameter
    qreal * zvar_ptr;		// pointer to the stochastic parameter
    
public:
    rlaborMarketFirstTax();		// constructor
    void loadParamset(QTextStream&);	// load parameterset from a file
    void iteration(const qint64&);	// perform one iteration 
    void initialize(void);		// initialize the model
};

#endif

// eof
