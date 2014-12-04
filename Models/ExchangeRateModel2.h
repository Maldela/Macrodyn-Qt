/****************************************************************************/
/* 																			*/
/* Module name:  ExchangeRateModel2.h										*/	
/* Contents:     Class definition of the class ExchangeRateModel2 			*/
/* Autor:		 Michael Meyer												*/
/* Date:		 Don M r 14 11:19:26 CET 2002								*/
/*																			*/
/* Modified:	 Don M r 14 11:18:06 CET 2002								*/
/* By:			 Michael Meyer												*/
/****************************************************************************/

#ifndef ExchangeRateModel2_H
#define ExchangeRateModel2_H

#include "baseModel.h"


class ExchangeRateModel2: public baseModel {

	protected:
    qreal thetat_0,st_1_0,st_2_0;		//initial value of state variables
    qreal eerealt_1_0,alphat_0,betat_0;	//initial value	of state variables
		
	//State Variables:

    qreal thetat;	// inflation rate
    qreal st_1;		// bond price in t-1
    qreal st_2;		// bond price in t-2
    qreal eerealt_1;	// qreal expected exchange rate in t-1
    qreal alphat;	// qreal wage
    qreal alphat_1;  // qreal wage in t-1
    qreal betat;		// qreal bonds
    qreal betat_1;	// qreal bonds in t-1
	
	//Variables:

    qreal rhoTilda;
    qreal *theta;	// expected inflation rate
    qreal *st_shift;
    qreal iet;		// expected interest rate
    qreal eqrealt;	// qreal exchange rate
    qreal ymaxt;		// capacity output
    qreal Lnt;		// notional labor demand
    qreal yLt;		// notional output
    qreal ydt;		// effective aggregate demand
    qreal c;			// propensity to consume
    qreal yt;		// output
    qreal yefft;		// effective demand
    qreal Lt;		// employment
    qreal st;		// bond price
    qreal *part_array; 	//Array containing past inflation rates
    qreal part, part_1, part_2, delta_part;	// price adjustment rates
    qreal uerate;

    qreal wart;		// wage adjustment rate
	

    qreal xtrate;
    qreal rxtrate;
    qreal ytrate;
    qreal Ltrate;
  


	//Parameters:

    int tau;		// length of memory 
    qreal A;			// scaling parameter of the production function
    qreal B;			// elasticity of production
    qreal Lmax;		// constant labor supply
    qreal rho;		// parameter of substitution (utility function)
    qreal delta;		// discount factor of the utility function
    qreal d;			// interest payment
    qreal E;			// foreign demand
    qreal g;		    // government demand
    qreal tax;		// proportional tax rate
    qreal gamma;		// price adjustment parameters
    qreal kappa;		// price adjustment parameters
    qreal lambda;	// wage adjustment parameters
    qreal mu;		// wage adjustment parameters
    qreal fi;		//foreign interest rate
    qreal deltaB;

	int warning;

    void theta_Init(qreal*);
    void st_Init(qreal*);
    void part_array_Init(qreal*);
    virtual qreal expectedInflationRate(const qint64);
	void expectedInterestRate();
    void qrealExchangeRate();
    qreal MaxCapacityOutput(const qreal&);
	void NotLaborDemand();
	void NotOutput();	
    void effaggrdemand(qreal&);
    void output(qreal&,qreal&,qreal&);
	void employment();
	void bondprice();
    void ratioRegime(qreal&,qreal&,char*);
    void wageAndpriceAdjust(qreal&,qreal&,qreal&,char*);
	void dynamics();
	

	
public:
    ExchangeRateModel2();					// default constructor
    virtual ~ExchangeRateModel2();			// destructor

	void initialize();						// initialize the model, define the initial state
    void loadParamset(ifstream&);			// load parameterset from a file
    void iteration(const qint64&);      		// perform one iteration
    qreal* setLabels(char*);					// return a pointer to a variable or a parameter specified by its name

    void sendStateSpace(int &,const qreal***);// return pointers to all model variables and the dimension of the model
    void sendParameters(int&,qreal**);		// write all parameters into an array and return the numbers of parameters
    void saveParamset(ofstream&);			// write parameterset into a file
    void saveParamsetWithNames(ofstream&);	// add parametset to printerfile (so far only for xpm)
    void receiveParameters(const qreal*);	// receive parameter values
    void printParamset();					// print parameterset on the screen    
    qreal* sendModelVar();
 
};
#endif

// eof


