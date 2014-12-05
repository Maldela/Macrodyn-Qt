///////////////////////////////////////////////////////////////////////////////
//
// $Header: michael.h,v 1.13 99/03/15 14:24:43 mmeyer Exp $
//
// Module name:		michael.h
// Contents:		Class definition of the class growth_model_m,
//			a dynamik growth model 
//
// Author:		Michael Meyer
// Last modified:	Mon Mar 15 12:40:17 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _MICHAEL_INCLUDED
#define _MICHAEL_INCLUDED

#include "baseModel.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model_m
// Derived from:	baseModel
// Purpose:		defines the basic growth_model
//
// Author:		Michael Meyer
// Last modified:	Mon Mar 15 12:40:08 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

class michael : public baseModel {

protected:
    			// original state variables
    qreal K;		// kapital
    qreal L;		// labour
    qreal k;		// transformed state variable k:=K/L

    qreal k_0;		// initial value
    qreal z;
    qreal z_0;
    qreal tau;
    qreal gamma;
    qreal rho;
    
    
    			// derived state variables
    qreal y;		// output
    qreal w;		// wage
    qreal r;		// rent


				// production function
    qreal ( * pf ) ( qreal k, qreal z, qreal b, qreal r );	
    				// a pointer to the production function
    qreal ( * pf_prime) ( qreal k, qreal z, qreal b, qreal r );
    				// and its derivative

    qreal pf_type;		// the current type of the production function
    qreal a, b, c, alpha;	// original parameters
    qreal A, B, C, D;		// parameters as ][0,1][ normalized input
    void pf_init ( void );	// maps (A,B,C) -> (a,b,c) and sets pf, pf_prime
    
    virtual qreal sf_y ( qreal );	// savings function for the entire economy
    qreal syr;			// parameter for sf_y
    
    
     			
    qreal sav_y;		// savings w.r.t. the output

    qreal n;		// exogenous growth of labour suppling people
   
     

public:
    michael();			// constructor
    void loadParamset(QDataStream&);	
    void saveParamset(QDataStream&);
    void printParamset();
    void iteration(const qint64&);
    void initialize();
    qreal* setLabels(const QString&);
    void sendStateSpace(int &,const qreal***);

    qreal* sendModelVar();		// for compatibity reasons only
    void sendParameters(int&,qreal**);
    void receiveParameters(const qreal*);
    virtual ~michael(void);
};

#endif //  _MICHAEL_INCLUDED

// eof
