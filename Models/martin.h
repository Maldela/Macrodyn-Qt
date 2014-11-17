#ifndef MARTIN_H
#define MARTIN_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/martin.h,v 1.1 2000/08/31 16:01:16 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  martin.h                                                     */
/* Contents:     Class definition of the classes martin, martin2, martin3     */
/*               and martin4 (UM)                                             */
/*                                                                            */
/* Last Modified: Tue May 14 15:36:56 MESZ 1996 (Uli Middelberg)	      */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"
#include "rand_var.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    martin                                                      */
/* Derived from:  baseModel                                                   */
/* Purpose:       defines martin's dynamical system                           */
/*                                                                            */
/* Last modified: 09.06.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

class martin : public baseModel {
protected:
    rand_var * zvar;		// ptr random function   
    
    real theta;			// parameters
    real rho;
    real beta;			// state variables
    real oldBeta;
    real k;
    real g;
    real beta0;			// initial values
    real k0;
    real g0;
				// derived variables
    real p;			// prices derived from beta
    real p0;			// initial value for p
    real perr;                  // prediction error inflation
    real perrp;			// prediction error prices
    real perrlnp;		// prediction error prices (log)
    
    virtual real savings(const real&);	// function for the savings
public:
    martin();			// constructor
    virtual ~martin();		// destructor;

    virtual void loadParamset(ifstream&);	
    virtual void saveParamset(ofstream&);
    virtual void printParamset();
    void initialize();
    virtual void iteration(const long&);
    real* sendModelVar();
    virtual real* setLabels(char*);
    void sendStateSpace(int &,const real***);
    virtual void sendParameters(int&,real**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    virtual void receiveParameters(const real*);// receive parameter values 
};


/******************************************************************************/
/*                                                                            */
/* Class name:    martin2                                                     */
/* Derived from:  martin                                                      */
/* Purpose:       defines martin's 2nd dynamical system                       */
/*                                                                            */
/* Last modified: Thu May 30 17:38:07 MESZ 1996 (Uli Middelberg)              */
/*                                                                            */
/******************************************************************************/

class martin2 : public martin {
protected:
    real lambda;				// Another parameter
    virtual real savings(const real&);
public:
    virtual void loadParamset(ifstream&);	
    virtual void saveParamset(ofstream&);
    virtual void printParamset();
    virtual real* setLabels(char*);
    virtual void sendParameters(int&,real**);		// as above
    virtual void receiveParameters(const real*);	// as above 
};

/******************************************************************************/
/*                                                                            */
/* Class name:    martin3                                                     */
/* Derived from:  martin2                                                      */
/* Purpose:       defines martin's 2rd dynamical system                       */
/*                                                                            */
/* Last modified: Thu May 30 17:38:07 MESZ 1996 (Uli Middelberg)              */
/*                                                                            */
/******************************************************************************/

class martin3 : public martin2 {
protected:
    real savings(const real&);
};


/******************************************************************************/
/*                                                                            */
/* Class name:    martin4                                                     */
/* Derived from:  martin2                                                     */
/* Purpose:       defines martin's 4th dynamical system                       */
/*                                                                            */
/* Last modified: Thu May 30 17:38:50 MESZ 1996 (Uli Middelberg)              */
/*                                                                            */
/******************************************************************************/

class martin4 : public martin2 {
protected:
    real a,b,c;			// parameters for the savings function
    real savings(const real&);
public:
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void printParamset();
    real* setLabels(char*);
    void sendParameters(int&,real**);		// as above
    void receiveParameters(const real*);	// as above 
};

#endif
