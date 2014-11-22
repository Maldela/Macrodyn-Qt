#ifndef OLGRAT_H
#define OLGRAT_H
// $Header :$

/******************************************************************************/
/*                                                                            */
/* Module name:  OlgRat.h                                                     */
/* Contents:     Class definition of the class OlgRat                         */
/*                                                                            */
/*                                                                            */
/* Last Modified: Fri Jun 21 14:23:35 MESZ 1996 (Uli Middelberg)	      */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    OlgRat                                                      */
/* Derived from:  baseModel                                                   */
/* Purpose:       defines leo's OlgRat dynamical system                       */
/*                                                                            */
/* Last modified: Fri Jun 21 14:23:35 MESZ 1996 (Uli Middelberg)              */
/*                                                                            */
/******************************************************************************/

class OlgRat : public baseModel {
protected:
    qreal g;			// parameters
    qreal tau;
    qreal s;
    qreal lambda;
    qreal m;			// state variable
    qreal m0;			// initial values

public:
    OlgRat();			// constructor
    virtual ~OlgRat();		// destructor;

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

class OlgRat2 : public OlgRat {

public:

virtual void iteration(const long&);

};

#endif

