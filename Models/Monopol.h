#ifndef MONOPOL_H
#define MONOPOL_H

/******************************************************************************/
/*                                                                            */
/* Module name:  Monopol.h                                                     */
/* Contents:     Class definition of the class Monopol                         */
/*                                                                            */
/*                                                                            */
/* Last Modified: Thu Sep 19 09:00:00 MESZ 1996 (Leo Kaas)	      */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    Monopol                                                      */
/* Derived from:  baseModel                                                   */
/* Purpose:       defines leo's Simple Monopol dynamical system                       */
/*                                                                            */
/* Last modified: Thu Sep 19 09:00:00 MESZ 1996 (Leo Kaas)              */
/*                                                                            */
/******************************************************************************/

class Monopol : public baseModel {
protected:
    real g;			// parameters
    real tau;			//
    real B;			//
    real eta;			//
    real alpha;			//
    real Lmax;			//
    real lambda;		//
    real mu;			//
    real delta;			//
    real cert;                  //
    
    real zeta;			// state variables
    real m;			//
    real z;
    real beta0;			// initial values
    real M0;			//
    real w0;
    real z0;
    
    real FF(const real L);	//
    real Finv(const real y);    //
    real gamma(void);		//
    real cpar(void);		//
    real xnot(void);		//
    real rewa(const real x);	//



public:
    Monopol();			// constructor
    virtual ~Monopol();		// destructor;

    virtual void loadParamset(ifstream&);	
    virtual void saveParamset(ofstream&);
    virtual void printParamset();
    void initialize();
    virtual void iteration(const long&);
    real* sendModelVar();
    virtual real* setLabels(char*);
    void sendStateSpace(int &,const real***);
    virtual void sendParameters(int&,real**);   // write all parameters into an 
                                                // array and return the numbers
				                // of parameters

    virtual void receiveParameters(const real*);// receive parameter values 
};


#endif

