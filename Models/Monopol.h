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
    qreal g;			// parameters
    qreal tau;			//
    qreal B;			//
    qreal eta;			//
    qreal alpha;			//
    qreal Lmax;			//
    qreal lambda;		//
    qreal mu;			//
    qreal delta;			//
    qreal cert;                  //
    
    qreal zeta;			// state variables
    qreal m;			//
    qreal z;
    qreal beta0;			// initial values
    qreal M0;			//
    qreal w0;
    qreal z0;
    
    qreal FF(const qreal L);	//
    qreal Finv(const qreal y);    //
    qreal gamma(void);		//
    qreal cpar(void);		//
    qreal xnot(void);		//
    qreal rewa(const qreal x);	//



public:
    Monopol();			// constructor
    virtual ~Monopol();		// destructor;

    virtual void loadParamset(ifstream&);	
    virtual void saveParamset(ofstream&);
    virtual void printParamset();
    void initialize();
    virtual void iteration(const long&);
    qreal* sendModelVar();
    virtual qreal* setLabels(char*);
    void sendStateSpace(int &,const qreal***);
    virtual void sendParameters(int&,qreal**);   // write all parameters into an 
                                                // array and return the numbers
				                // of parameters

    virtual void receiveParameters(const qreal*);// receive parameter values 
};


#endif

