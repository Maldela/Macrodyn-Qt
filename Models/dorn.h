#ifndef DORN_H
#define DORN_H
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/dorn.h,v 1.1 2000/08/31 15:26:05 mmeyer Exp $
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
    qreal p0,w0,s0,B0,x0,i0,xe0,ie0;
    int tau; 
    qreal *p;
    qreal pexp_t;                      
    qreal Im;      // import            
    qreal ytz;     // notional production
    qreal ytf;     // production for full employment
    qreal ytd;
    qreal zt;      // labor demand
    qreal xt;      // actuell exchange rate
    qreal pt;      // goods price
    qreal A;       // scaling parameter of the production function
    qreal B;       // elasticity of production
    qreal wt;      // nominal wage
    qreal Lmax;    // constant labor supply
    qreal Lt;      // employment
    qreal yt;      // production 
    qreal tax;     // proportional tax rate
    qreal iet;     // expected interest rate
    qreal delta;   // discount factor of the utility function
    qreal rho;     // parameter of substitution (utility function)
    qreal st;      // equilibrium bond price
    qreal d;       // interest payment
    qreal Bt;      // bonds at the end of period t-1
    qreal deltaB;  // new bonds
    qreal pe;      // expected goods price 
    qreal xtdy;    // goods demanded by the young household 
    qreal stv;     // old equilibrium bond price
    qreal it;      // interest rate  
    qreal g;       // government demand
    qreal xet;      // expected exchange rate
    qreal fi;      // foreign interest                 
    qreal V;
    qreal W;
    qreal Pit;
    qreal Ytnet;
    qreal Yt;
    qreal deltayt;
    qreal xetv;
    qreal gamma; 
    qreal kappa;
    qreal lambda;
    qreal mu;
               
public:
    dorn();			       // default constructor
    virtual ~dorn();                   // destructor
    void pInit(qreal*);		       
    qreal pexpBerechnung(const long);
    void dynamics(qreal *,const long);
    void labordemand ();
    void profit();
    void income();
    void aggregateDemand();
    void employment();
    void fullProduction();
    void notionalProduction();
    void production();
    void output();
    void pBerechnung();
    void WageAdjustment();
    void newBonds();
    void ExpectedInterestRate();
    void goodsDemand();
    void bondMarket();
    void ExchangeRate();
    void pwBerechnung();
    void loadParamset(ifstream&);	      // load parameterset from a file
    void saveParamset(ofstream&);	      // write parameterset into a file
    void saveParamsetWithNames(ofstream&);    // add parametset to printerfile (so far only for xpm)
    void receiveParameters(const qreal*);      // receive parameter values
    void printParamset();		      // print parameterset on the screen    
    virtual void iteration(const long&);      // perform one iteration 
    qreal* setLabels(char*);		      // return a pointer to a variable
					      // or a parameter specified by its
					      // name
    void initialize();			      // initialize the model, define
					      // the initial state
    void sendStateSpace(int &,const qreal***); // return pointers to all model
					      // variables and the dimension
					      // of the model
    void sendParameters(int&,qreal**);         // write all parameters
                                              // into an array and return the numbers
				              // of parameters
};
#endif

// eof


