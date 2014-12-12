#ifndef PUREEXCHANGE_H
#define PUREEXCHANGE_H
//$Header: pureExchange.h
/******************************************************************************/
/*                                                                            */
/* Module name:  pureExchange.h                                               */
/* Contents:     Class definition of the class pureExchange 	              */
/*                                                                            */
/* Last Modified: 07.09.1995 (Markus Lohmann)                                 */
/* Modified:      07.09.95 excOlgAdapt and excOlgGeoExp added (M. Lohmann)    */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class pureExchange : public baseModel {
  protected:
	qreal job_type;				// current type of job (used for jbo 83)
	qreal err;					// error function : theta_t - theE_Old
	qreal sf_type;				// the current type of the savings function
    qreal theta_e_m1;            // initial inflation rate for period -1                
	qreal theta_e_null;			// initial inflation rate for period 0
	qreal gamma;					// gross rate of currency growth
	qreal p0;					// initial price
	qreal w1, w2;				// initial endowment for t,t+1	
	qreal g0;					// initial parameter for exp.Infl.rate OLS
	qreal g_tm1;					// parameter for exp.Infl.rate OLS
	qreal rho, delta;			// parameter for the savings function
    qreal theta_tm1;		        // inflation rate in t-1
    qreal theta_t;		        // inflation rate in t
	qreal theE_Old;				// expected inflation rate for period t
	qreal theE_You;			// expected inflation rate for period t+1
	qreal thetaOld;
	qreal theE_Old2;
	qreal theE_You2;
	qreal pt;					// price in t
	qreal p_tm1;					// price in t-1
    void expInflRateYoung();  	// algorithm for theE_You                      
    qreal (*savingsFunc)(qreal,qreal,qreal,qreal,qreal);    //pointer to the savings function  
	void sf_init ();			// sets savingsFunc

public:
    pureExchange();			   		 // default constructor
    virtual ~pureExchange();		    // destructor   
    void loadParamset(QTextStream&);	    // load parameterset from a file
    void saveParamset(QTextStream&);	    // write parameterset into a file
    void printParamset();		    	// print parameterset on the screen
    void iteration(const qint64&);    	// perform one iteration    
	qreal* setLabels(const QString&);		    	// return a pointer to a variable
					    				// or a parameter specified by its
					    				// name
    void initialize();			    	// initialize the model, define
					    				// the initial state
    qreal* sendModelVar();		    	// return the main model variable
    void sendStateSpace(int &, QList<qreal *> *); 	// return pointers to all model
					    						// variables and the dimension
					    						// of the model
    void sendParameters(int&,qreal**); 	// write all parameters
                                		// into an array and return the numbers
										// of parameters
    void receiveParameters(const QList<qreal>&); 		// receive parameter values 
};

#endif // PUREEXCHANGE_H

// eof
