#ifndef defaultStarSTARMODEL_H
#define defaultStarSTARMODEL_H
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/PublicModels/RCS/defaultStarModel.h,v 1.5 1998/07/21 13:11:07 mmueller Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  defaultStarModel.h                                           */
/* Contents:     Class definition of the class defaultStarModel               */
/*               This model is taken from the discussion paper No. 288 of the */
/*               university of bielefeld: "Dynamic Complexity in a Keynesian  */
/*               Macroeconomic Model" by Volker Boehm, Markus Lohmann and     */
/*               Hans-Walter Lorenz                                           */
/*                                                                            */
/* Modified:     09.11.1994 (Markus Lohmann)                                  */
//
// Revised:	 Mon Jan 13 15:48:01 MET 1997
// By:		 Uli Middelberg
// Changes:	 implementation of stochastic shocks
//
// Revised:	 Tue Jul 21 12:50:04 METDST 1998
// By:		 Marc Mueller
// Changes:	 function saveParamsetWithNames implemented
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"
#include "rand_var.h"

class defaultStarModel : public baseModel {

  protected:
    qreal A,B;                             // production function parameters
    qreal D;                               // labor supply function parameters
    qreal gamm,kappa;                      // price adjustment parameters
    qreal my,lambda;                       // wage adjustment parameters
    qreal w0,p0,m0;                        // initial values
    qreal wtreal,mtreal,ptrate,wtrate;     // real wage, real balance
    qreal Lmax;                            // maximum labour supply Parameter
    qreal Lstar;                           // maximum labour supply
    qreal ymax;                            // maximum capacity output
    qreal rho, delta;                      // utility function parameters
    qint64 tau;                              // length of expectations memory
    qreal rhoTilda;
    qreal g,tax;                           // government demand, tax rate
    qreal employment;                      // actual employment
    qreal beta;
    qreal *theta;
    qreal theta1;
    qreal output;                          // actual output
    qreal noK,noI,noC;			  //counter state K/I/C
    qreal KCI;				  //state variable
    qreal relNoK,relNoC,relNoI;		  // proportion of state K/I/C
    


    qreal LstarFunction(const qreal&, const qreal&, const qreal&);
    qreal prodFunction(const qreal&);
    virtual qreal expectedInflationRate(const qint64);
    void notProd(qreal&,qreal&);
    void notCom(qreal&,qreal&,qreal&);
    void empAndOut(qreal&,qreal&);
    void detDiseq(qreal&,qreal&,char*);
    void wageAndPrice(qreal&,qreal&,qreal&,char*);
    void dynamics();                        
public:
    defaultStarModel();			    // defaultStar constructor
    virtual ~defaultStarModel();		// destructor
    
    void loadParamset(QTextStream& );	    // load parameterset from a file
    void saveParamset(QTextStream& );	    // write parameterset into a file
    void saveParamsetWithNames(QTextStream& );  // add parametset to printerfile (so far only for xpm)
    void printParamset();		    // print parameterset on the screen
    
    void iteration(const qint64&);    // perform one iteration
    qreal* setLabels(const QString&);		    // return a pointer to a variable
					    // or a parameter specified by its
					    // name
    void initialize();			    // initialize the model, define
					    // the initial state
    void thetaInit(qreal*);
    void getTheta(qreal*);
    void setTheta(qreal*);
    qreal* sendModelVar();		    // return the main model variable
    void sendStateSpace(int &, QList<qreal *> *); // return pointers to all model
					    // variables and the dimension
					    // of the model
    void sendParameters(int&,qreal**); // write all parameters
                                      // into an array and return the numbers
				      // of parameters
    void receiveParameters(const QList<qreal>&); // receive parameter values
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rlogisticMap
// Derived from:        logisticMap
// Purpose:             defines the logistic map with random variables
//
// Author:              Uli Middelberg
// Last modified:       Tue Jan 21 15:32:30 MET 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class rdefaultStarModel : public defaultStarModel {
private:
    char zvar_expr[256];        // expression which defines a random variable
    rand_var * zvar;            // a random variable    
    char zvar_name[256];        // name of the stochastified parameter
    qreal * zvar_ptr;		// pointer to the stochastic parameter
    
public:
    rdefaultStarModel();			// constructor
    void loadParamset(QTextStream& );	// load parameterset from a file
    void iteration(const qint64&);	// perform one iteration
    void initialize(void);		// initialize the model
};

#endif

// eof
