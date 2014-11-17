#ifndef DUOPOL_H
#define DUOPOL_H

/******************************************************************************/
/*                                                                            */
/* Module name:  Duopol.h                                                     */
/* Contents:     Class definition of the class Duopol                         */
/*                                                                            */
/*                                                                            */
/* Last Modified: Sun Oct  6 15:26:21 MEZ 1996 (Leo Kaas)	                  */
/*                                                                            */
/*****************************************************************************/

#include "baseModel.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    Duopol                                                      */
/* Derived from:  baseModel                                                   */
/* Purpose:       defines leo's Simple Duopol dynamical system                */
/*                                                                            */
/* Last modified: Sun Oct  6 15:26:29 MEZ 1996 (Leo Kaas)                     */
/*                                                                            */
/******************************************************************************/

class Duopol : public baseModel {
protected:
    real g;			// parameters
    real tau;			//
    real B1;			//
    real B2;			//
    real eta1;			//
    real eta2;			//
    real alpha1;		//
    real alpha2;		//
    real lambda1;		//
    real lambda2;		//
    real Lmax;			//  
    real mu;			//
    real dep;                   //
    real rho;                   //
    real delta;			//
    real cert;                  //
    
    real zeta1;			// state variables (real)
    real zeta2;
    real z1;
    real z2;
    real m;			//
    real pi;
    
    real beta10;		// initial values (nominal)
    real beta20;
    real z10;
    real z20;
    real M0;			//
    real w0;
    real Pi0;
    
    real FF(const real L, const int firm);      
    real Finv(const real y, const int firm); 
    real nullst(const real gamc, const real alpha, const real eta);   
    real Gamma(const int firm);		
    real cpar(const int firm);		
    real xnot(const int firm);		
    real rewa(const real x, const int firm);
    real cprop(const real theta);
    

public:
    Duopol();			// constructor
    virtual ~Duopol();		// destructor;

    virtual void loadParamset(ifstream&);	
    virtual void saveParamset(ofstream&);
    virtual void printParamset();
    virtual void initialize();
    virtual void iteration(const long&);
    virtual real* sendModelVar();
    virtual real* setLabels(char*);
    virtual void sendStateSpace(int &,const real***);
    virtual void sendParameters(int&,real**);   // write all parameters into an 
                                                // array and return the numbers
				                // of parameters

    virtual void receiveParameters(const real*);// receive parameter values 
};

class Duopol2 : public Duopol {

public:

    virtual void iteration(const long&);

};

#endif

