#ifndef DEFAULTMODEL_H
#define DEFAULTMODEL_H
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/PublicModels/RCS/defaultModel.h,v 1.5 1998/07/21 13:11:07 mmueller Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  defaultModel.h                                               */
/* Contents:     Class definition of the class defaultModel                   */
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

class defaultModel : public baseModel {
  protected:
    real A,B;                             // production function parameters
    real gamm,kappa;                      // price adjustment parameters   
    real my,lambda;                       // wage adjustment parameters    
    real w0,p0,m0;                        // initial values                
    real wtreal,mtreal,ptrate,wtrate;     // real wage, real balance       
    real Lmax;                            // maximum labour supply
    real ymax;                            // maximum capacity output
    real rho, delta;                      // utility function parameters   
    int tau;                              // length of expectations memory
    real rhoTilda;
    real g,tax;                           // government demand, tax rate   
    real employment;                      // actual employment             
    real beta;
    real *theta;
    real output;                          // actual output
    
    real prodFunction(const real&);
    virtual real expectedInflationRate(const long);                
    void notProd(real&,real&);                        
    void notCom(real&,real&,real&);                         
    void empAndOut(real&,real&);
    void detDiseq(real&,real&,char*);                 
    void wageAndPrice(real&,real&,real&,char*);            
    void dynamics();                        
public:
    defaultModel();			    // default constructor
    virtual ~defaultModel();		    // destructor
    
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamset(ofstream&);	    // write parameterset into a file
    void saveParamsetWithNames(ofstream&);  // add parametset to printerfile (so far only for xpm)
    void printParamset();		    // print parameterset on the screen
    
    virtual void iteration(const long&);    // perform one iteration 
    real* setLabels(char*);		    // return a pointer to a variable
					    // or a parameter specified by its
					    // name
    void initialize();			    // initialize the model, define
					    // the initial state
    void thetaInit(real*);
    void getTheta(real*);
    void setTheta(real*);
    real* sendModelVar();		    // return the main model variable
    void sendStateSpace(int &,const real***); // return pointers to all model
					    // variables and the dimension
					    // of the model
    void sendParameters(int&,real**); // write all parameters
                                      // into an array and return the numbers
				      // of parameters
    void receiveParameters(const real*); // receive parameter values 
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

class rdefaultModel : public defaultModel {
private:
    char zvar_expr[256];        // expression which defines a random variable
    rand_var * zvar;            // a random variable    
    char zvar_name[256];        // name of the stochastified parameter
    real * zvar_ptr;		// pointer to the stochastic parameter
    
public:
    rdefaultModel();			// constructor
    void loadParamset(ifstream&);	// load parameterset from a file
    void iteration(const long&);	// perform one iteration 
    void initialize(void);		// initialize the model
};

#endif

// eof
