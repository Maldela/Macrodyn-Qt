/****************************************************************************/
/* 																			*/
/* Module name:  ExchangeRateModel2.h										*/	
/* Contents:     Class definition of the class ExchangeRateModel2 			*/
/* Autor:		 Michael Meyer												*/
/* Date:		 Don Mär 14 11:19:26 CET 2002								*/
/*																			*/
/* Modified:	 Don Mär 14 11:18:06 CET 2002								*/
/* By:			 Michael Meyer												*/
/****************************************************************************/

#ifndef ExchangeRateModel2_H
#define ExchangeRateModel2_H

#include "baseModel.h"


class ExchangeRateModel2: public baseModel {

	protected:
	real thetat_0,st_1_0,st_2_0;		//initial value of state variables
	real eerealt_1_0,alphat_0,betat_0;	//initial value	of state variables		                                                            
		
	//State Variables:

	real thetat;	// inflation rate	
	real st_1;		// bond price in t-1
	real st_2;		// bond price in t-2
	real eerealt_1;	// real expected exchange rate in t-1
	real alphat;	// real wage 
	real alphat_1;  // real wage in t-1
	real betat;		// real bonds
	real betat_1;	// real bonds in t-1
	
	//Variables:

	real rhoTilda;
	real *theta;	// expected inflation rate	
	real *st_shift;
	real iet;		// expected interest rate
	real erealt;	// real exchange rate
	real ymaxt;		// capacity output
	real Lnt;		// notional labor demand
	real yLt;		// notional output
	real ydt;		// effective aggregate demand
	real c;			// propensity to consume
	real yt;		// output
	real yefft;		// effective demand
	real Lt;		// employment
	real st;		// bond price
	real *part_array; 	//Array containing past inflation rates
	real part, part_1, part_2, delta_part;	// price adjustment rates 
	real uerate;

	real wart;		// wage adjustment rate	
	

	real xtrate;
	real rxtrate;
	real ytrate;
	real Ltrate;
  


	//Parameters:

    int tau;		// length of memory 
    real A;			// scaling parameter of the production function
	real B;			// elasticity of production
	real Lmax;		// constant labor supply
	real rho;		// parameter of substitution (utility function)
	real delta;		// discount factor of the utility function
	real d;			// interest payment
	real E;			// foreign demand
	real g;		    // government demand
	real tax;		// proportional tax rate
	real gamma;		// price adjustment parameters
	real kappa;		// price adjustment parameters
	real lambda;	// wage adjustment parameters
	real mu;		// wage adjustment parameters	
	real fi;		//foreign interest rate
	real deltaB;	

	int warning;

    void theta_Init(real*);
	void st_Init(real*);
	void part_array_Init(real*);
	virtual real expectedInflationRate(const long);                
	void expectedInterestRate();
	void realExchangeRate();
	real MaxCapacityOutput(const real&);
	void NotLaborDemand();
	void NotOutput();	
	void effaggrdemand(real&);
	void output(real&,real&,real&);
	void employment();
	void bondprice();
	void ratioRegime(real&,real&,char*);
	void wageAndpriceAdjust(real&,real&,real&,char*);                
	void dynamics();
	

	
public:
    ExchangeRateModel2();					// default constructor
    virtual ~ExchangeRateModel2();			// destructor

	void initialize();						// initialize the model, define the initial state
    void loadParamset(ifstream&);			// load parameterset from a file
	void iteration(const long&);      		// perform one iteration 
    real* setLabels(char*);					// return a pointer to a variable or a parameter specified by its name

    void sendStateSpace(int &,const real***);// return pointers to all model variables and the dimension of the model
    void sendParameters(int&,real**);		// write all parameters into an array and return the numbers of parameters
    void saveParamset(ofstream&);			// write parameterset into a file
    void saveParamsetWithNames(ofstream&);	// add parametset to printerfile (so far only for xpm)
    void receiveParameters(const real*);	// receive parameter values
    void printParamset();					// print parameterset on the screen    
	real* sendModelVar();
 
};
#endif

// eof


