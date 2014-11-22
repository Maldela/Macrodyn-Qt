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
    qreal A,B;                             // production function parameters
    qreal deltaP;			  // depreciationrate
    qreal Lmax;                            // maximum labour supply
  //Consumer
    qreal betaS,betaW;
    qreal rhoS,rhoW,deltaS,deltaW;         // utility function parameters   
    qreal rhoTildaS,rhoTildaW;
    int  tauS,tauW;                       // length of expectations memory
  //Government
    qreal g,taxS,taxW;                     // government demand, tax rates   
  //Velocity
    qreal gamm,kappa;                      // price adjustment parameters   
    qreal lambda,mu;                       // wage adjustment parameters    
  //Initial state
    qreal w0,mS0,mW0,omega0,d0,theta0;     // initial values                
  //Condition variables
    qreal wtqreal;			// qreal wage
    qreal mtqrealS,mtqrealW;	        // qreal moneybalance ShareholderWorker
    qreal omegat,dt;    			// unsold output,qreal dividend
    qreal ptrate,wtrate;                 // qreal wage, qreal balance       
    qreal employment;                    // actual employment   
    qreal output;                        // actual sold output
    qreal *theta;			// pointer on recent inflationrates
  //Model functions
    qreal laborDemand();                 // national demand for Labor
    qreal actualEmployment(qreal&);
    qreal detSigmaL(qreal&);              // percentage surplus of the labormarket
    qreal detWtRate(qreal&);	        // wage adjustment 
    virtual qreal expectedInflationRateW(const long);
    qreal consumptionPropensityW(qreal&); 
    qreal demandYoungW(qreal&);           // desired consum of the young workers
    virtual qreal expectedInflationRateS(const long);                
    qreal consumptionPropensityS(qreal&);
    qreal demandYoungS(qreal&); 
    qreal aggregateDemand(qreal&,qreal&);
    qreal productionFunction(const qreal&);// compute effectiv production
    qreal actualOutput(qreal&,qreal&);      // actual sold output
    qreal remainingOutputYoung();
    qreal actualConsumptionYoungS(qreal&,qreal&,qreal&);
    qreal actualConsumptionYoungW(qreal&,qreal&,qreal&);
    qreal detSigmaC(qreal&,qreal&);      //percentage signal for price modification
    qreal actualInflationrate(qreal&,qreal&);    
    void dynamics(qreal&,qreal&,qreal&);                        
public:
    laborMarketFirst();			 // constructor
    virtual ~laborMarketFirst();	 // destructor

    void thetaInit(qreal*);		 // vector of recent inflationrates
    void initialize();			 // initialize the model, define
					 // the initial state
    void loadParamset(ifstream&);	 // load parameterset from a file
    void saveParamset(ofstream&);	 // write parameterset into a file
    void printParamset();		 // print parameterset on the screen
    void sendParameters(int&,qreal**);    // write all parameters into an array
                                         // and return the numbers of parameters
    void receiveParameters(const qreal*); // receive parameter values 

    qreal* setLabels(char*);		 // return a pointer to a variable or
					 // a parameter specified by its name
    qreal* sendModelVar();		 // return the main model variable
    void sendStateSpace(int &,const qreal***); // return pointers to all model
					 // variables and the dimension
					 // of the model
    virtual void iteration(const long&); // perform one iteration 
};
#endif
