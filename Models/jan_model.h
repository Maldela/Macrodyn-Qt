///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/jan_model.h,v 1.1 2000/08/31 15:57:44 mmeyer Exp $
//
// Module name:		jan_model.h
// Contents:		Class definition of the class logistic
//
// Author:		Jan Wenzelburger
// Last modified:	Fri 
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _JAN_MODEL_INCLUDED
#define _JAN_MODEL_INCLUDED

#include "baseModel.h"

///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/jan_model.h,v 1.1 2000/08/31 15:57:44 mmeyer Exp $
//
// Class name:	        jan_model
// Derived from:	baseModel
// Purpose:		Test
//
// Author:		Jan
// Last modified:	Fri 
// By:		
//
///////////////////////////////////////////////////////////////////////////////

class jan_model : public baseModel {

protected:
    real alpha;				// parameter
    real delta;                         // parameter 
    real x;				// state variable
    real x0;				// initial value for x
   

public:
    jan_model();			// constructor
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void printParamset();
    void iteration(const long&);
    void initialize();
    real* sendModelVar();
    real* setLabels(char*);
    void sendStateSpace(int &,const real***);
    void sendParameters(int&,real**); 	// write all parameters
                                	// into an array and return the 
                                	// numbers of parameters
                                	
    void receiveParameters(const real*);// receive parameter values 
};


#endif	// _JAN_MODEL_INCLUDED

// eof
