#ifndef DORN_H
#define DORN_H
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/dorn_1.h,v 1.1 2000/08/31 15:48:10 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  dorn.h                                                       */
/* Contents:     Class definition of the class dorn                           */
/* Modified:    			                                      */
/*									      */
/* Revised:								      */
/* By:		 							      */
/* Changes:	 		      					      */
/*									      */
/* Revised:				      				      */
/* By:		               						      */
/* Changes:	                                             		      */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"
#include "rand_var.h"

class dorn: public baseModel {
  protected:
    real p0,w0,s0,B0,ie0,xe0;				                                                            
    int tau; 
    real *p;
    real pexp_t;                                  
    real zt;      // labor demand
    real xt;      // actuell
    real pt;      // goods price
    real A;       // scaling parameter of the production function
    real B;       // elasticity of production
    real wt;      // nominal wage
    real Lmax;    // constant labor supply
    real Lt;      // employment
    real Pit;     // profit
    real yt;      // production 
    real Ytnet;   // nominal net income
    real tax;     // proportional tax rate
    real iet;      // expected interest rate
    real delta;   // discount factor of the utility function
    real rho;     // parameter of substitution (utility function)
    real st;      // equilibrium bond price
    real d;       // interest payment
    real Bt;      // bonds at the end of period t-1
    real deltaB;  // new bonds
    real pe;      // expected goods price 
    real xtdy;    // goods demanded by the young household 
    real stv;     // old equilibrium bond price
    real it;      // interest rate  
    real ytd;     // aggregate goods demand
    real g;       // government demand
    real Yt;      // actual transaction on the domestic goods market
    real deltayt; // export/import
    real xet;      // expected exchange rate
    real fi;      // foreign interest                 
    real gamma; 
    real kappa;
    real lambda;
    real mu;
               
public:
    dorn();			       // default constructor
    virtual ~dorn();                   // destructor
    void pInit(real*);		       
    real pexpBerechnung(const long);
    void dynamics(real *,const long);
    void labordemand ();
    void employment();
    void production();
    void profit();
    void income();
    void ExpectedInflationRate();
    void ExpectedInterestRate();
    void goodsDemand();
    void bondMarket();
    void aggregateDemand();
    void output();
    void ExchangeRate();
    void pBerechnung();
    void WageAdjustment();
    void loadParamset(ifstream&);	      // load parameterset from a file
    void saveParamset(ofstream&);	      // write parameterset into a file
    void saveParamsetWithNames(ofstream&);    // add parametset to printerfile (so far only for xpm)
    void receiveParameters(const real*);      // receive parameter values
    void printParamset();		      // print parameterset on the screen    
    virtual void iteration(const long&);      // perform one iteration 
    real* setLabels(char*);		      // return a pointer to a variable
					      // or a parameter specified by its
					      // name
    void initialize();			      // initialize the model, define
					      // the initial state
    void sendStateSpace(int &,const real***); // return pointers to all model
					      // variables and the dimension
					      // of the model
    void sendParameters(int&,real**);         // write all parameters
                                              // into an array and return the numbers
				              // of parameters
};
#endif

// eof
