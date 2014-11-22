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
    qreal g;			// parameters
    qreal tau;			//
    qreal B1;			//
    qreal B2;			//
    qreal eta1;			//
    qreal eta2;			//
    qreal alpha1;		//
    qreal alpha2;		//
    qreal lambda1;		//
    qreal lambda2;		//
    qreal Lmax;			//  
    qreal mu;			//
    qreal dep;                   //
    qreal rho;                   //
    qreal delta;			//
    qreal cert;                  //
    
    qreal zeta1;			// state variables (qreal)
    qreal zeta2;
    qreal z1;
    qreal z2;
    qreal m;			//
    qreal pi;
    
    qreal beta10;		// initial values (nominal)
    qreal beta20;
    qreal z10;
    qreal z20;
    qreal M0;			//
    qreal w0;
    qreal Pi0;
    
    qreal FF(const qreal L, const int firm);      
    qreal Finv(const qreal y, const int firm); 
    qreal nullst(const qreal gamc, const qreal alpha, const qreal eta);   
    qreal Gamma(const int firm);		
    qreal cpar(const int firm);		
    qreal xnot(const int firm);		
    qreal rewa(const qreal x, const int firm);
    qreal cprop(const qreal theta);
    

public:
    Duopol();			// constructor
    virtual ~Duopol();		// destructor;

    virtual void loadParamset(ifstream&);	
    virtual void saveParamset(ofstream&);
    virtual void printParamset();
    virtual void initialize();
    virtual void iteration(const long&);
    virtual qreal* sendModelVar();
    virtual qreal* setLabels(char*);
    virtual void sendStateSpace(int &,const qreal***);
    virtual void sendParameters(int&,qreal**);   // write all parameters into an 
                                                // array and return the numbers
				                // of parameters

    virtual void receiveParameters(const qreal*);// receive parameter values 
};

class Duopol2 : public Duopol {

public:

    virtual void iteration(const long&);

};

#endif

