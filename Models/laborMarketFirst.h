#ifndef LABORMARKETFIRST_H
#define LABORMARKETFIRST_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/laborMarketFirst.h,v 1.1 2000/08/31 15:58:03 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  LaborMarketFirst.h                                           */
/* Contents:     Class definition of the class LaborMarketFirst               */
/*                                                                            */
/* Modified:     24.11.1995 (Marc Mueller)                                    */
/* Last Modified:08.12.1995                                                   */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class laborMarketFirst : public baseModel {
  protected:
  //Parameters
    real A,B;                             // production function parameters
    real deltaP;			  // depreciationrate
    real Lmax;                            // maximum labour supply
  //Consumer
    real betaS,betaW;
    real rhoS,rhoW,deltaS,deltaW;         // utility function parameters   
    real rhoTildaS,rhoTildaW;
    int  tauS,tauW;                       // length of expectations memory
  //Government
    real g,taxS,taxW;                     // government demand, tax rates   
  //Velocity
    real gamm,kappa;                      // price adjustment parameters   
    real lambda,mu;                       // wage adjustment parameters    
  //Initial state
    real w0,mS0,mW0,omega0,d0,theta0;     // initial values                
  //Condition variables
    real wtreal;			// real wage
    real mtrealS,mtrealW;	        // real moneybalance ShareholderWorker
    real omegat,dt;    			// unsold output,real dividend
    real ptrate,wtrate;                 // real wage, real balance       
    real employment;                    // actual employment   
    real output;                        // actual sold output
    real *theta;			// pointer on recent inflationrates
  //Model functions
    real laborDemand();                 // national demand for Labor
    real actualEmployment(real&);
    real detSigmaL(real&);              // percentage surplus of the labormarket
    real detWtRate(real&);	        // wage adjustment 
    virtual real expectedInflationRateW(const long);
    real consumptionPropensityW(real&); 
    real demandYoungW(real&);           // desired consum of the young workers
    virtual real expectedInflationRateS(const long);                
    real consumptionPropensityS(real&);
    real demandYoungS(real&); 
    real aggregateDemand(real&,real&);
    real productionFunction(const real&);// compute effectiv production
    real actualOutput(real&,real&);      // actual sold output
    real remainingOutputYoung();
    real actualConsumptionYoungS(real&,real&,real&);
    real actualConsumptionYoungW(real&,real&,real&);
    real detSigmaC(real&,real&);      //percentage signal for price modification
    real actualInflationrate(real&,real&);    
    void dynamics(real&,real&,real&);                        
public:
    laborMarketFirst();			 // constructor
    virtual ~laborMarketFirst();	 // destructor

    void thetaInit(real*);		 // vector of recent inflationrates
    void initialize();			 // initialize the model, define
					 // the initial state
    void loadParamset(ifstream&);	 // load parameterset from a file
    void saveParamset(ofstream&);	 // write parameterset into a file
    void printParamset();		 // print parameterset on the screen
    void sendParameters(int&,real**);    // write all parameters into an array
                                         // and return the numbers of parameters
    void receiveParameters(const real*); // receive parameter values 

    real* setLabels(char*);		 // return a pointer to a variable or
					 // a parameter specified by its name
    real* sendModelVar();		 // return the main model variable
    void sendStateSpace(int &,const real***); // return pointers to all model
					 // variables and the dimension
					 // of the model
    virtual void iteration(const long&); // perform one iteration 
};
#endif
