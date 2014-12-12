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
    
    qreal theta;			// parameters
    qreal rho;
    qreal beta;			// state variables
    qreal oldBeta;
    qreal k;
    qreal g;
    qreal beta0;			// initial values
    qreal k0;
    qreal g0;
				// derived variables
    qreal p;			// prices derived from beta
    qreal p0;			// initial value for p
    qreal perr;                  // prediction error inflation
    qreal perrp;			// prediction error prices
    qreal perrlnp;		// prediction error prices (log)
    
    virtual qreal savings(const qreal&);	// function for the savings
public:
    martin();			// constructor
    virtual ~martin();		// destructor;

    virtual void loadParamset(QTextStream&);	
    virtual void saveParamset(QTextStream&);
    virtual void printParamset();
    void initialize();
    virtual void iteration(const qint64&);
    qreal* sendModelVar();
    virtual qreal* setLabels(const QString&);
    void sendStateSpace(int &, QList<qreal *> *);
    virtual void sendParameters(int&,qreal**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    virtual void receiveParameters(const QList<qreal>&);// receive parameter values 
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
    qreal lambda;				// Another parameter
    virtual qreal savings(const qreal&);
public:
    virtual void loadParamset(QTextStream&);	
    virtual void saveParamset(QTextStream&);
    virtual void printParamset();
    virtual qreal* setLabels(const QString&);
    virtual void sendParameters(int&,qreal**);		// as above
    virtual void receiveParameters(const QList<qreal>&);	// as above 
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
    qreal savings(const qreal&);
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
    qreal a,b,c;			// parameters for the savings function
    qreal savings(const qreal&);
public:
    void loadParamset(QTextStream&);	
    void saveParamset(QTextStream&);
    void printParamset();
    qreal* setLabels(const QString&);
    void sendParameters(int&,qreal**);		// as above
    void receiveParameters(const QList<qreal>&);	// as above 
};

#endif
