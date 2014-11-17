#ifndef CAPOLG_H
#define CAPOLG_H
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/capOlg.h,v 1.1 2000/08/31 15:24:04 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  capOlg.h                                                */
/* Contents:     Class definition of the class capOlg                    */
/*                                                                            */
/* Last Modified: 04.07.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class capOlg : public baseModel {
  protected:
    real A,B,d;                           // production function parameters
    real k0,r0;                           // initial values                
    real wt;                              // real wage
    real n;      
    real ymax;                            // maximum capacity output
    real rho, delta;                      // utility function parameters   
    int tau;                              // length of expectations memory
    real rhoTilda;
    real rt;		                  // return rate
    real kt;                              // capital
    real st;
    real *rVec;
    real indivSave;                       // individual savings
    
    real prodFunction(const real&);
    virtual real expReturnRate(const long);                
    real savingsFunc(real&);                         
    void rShift();
public:
    capOlg();			    // default constructor
    virtual ~capOlg();		    // destructor
    
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamset(ofstream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    
    virtual void iteration(const long&);    // perform one iteration 
    real* setLabels(char*);		    // return a pointer to a variable
					    // or a parameter specified by its
					    // name
    void initialize();			    // initialize the model, define
					    // the initial state
    void rInit(real*);
    real* sendModelVar();		    // return the main model variable
    void sendStateSpace(int &,const real***); // return pointers to all model
					    // variables and the dimension
					    // of the model
    void sendParameters(int&,real**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const real*); // receive parameter values 
};

class capOlgAdapt : public capOlg {
protected:
    real expectedReturnRate(const long);
    real oldExpectations;                   // used to save the expected 
				            // return rate for the last period
    real eta;                               // weight parameter
public:
    capOlgAdapt();                          // default constructor
    real* setLabels(char*);
    void iteration(const long&);    // perform one iteration 
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamset(ofstream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    void sendParameters(int&,real**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const real*);// receive parameter values 
};


/******************************************************************************/
/*                                                                            */
/* Class name:   capOlgGeoExp                                                 */
/* Contents:     Class definition of the class capOlgGeoExp                   */
/*               This model is derived from the capOlg. The difference   */
/*               is that the expected infalation rate is computed by using    */
/*               a geometric distribution on the past tau inflation rates.    */
/*                                                                            */
/* Last Modified: 07.09.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

class capOlgGeoExp : public capOlg {
protected:
    real expectedReturnRate(const long);
    real eta;                               // parameter of the geometric lag
				            // distribution
    real etaTilda;
    void initialize();
public:
    capOlgGeoExp();                         // default constructor
    void iteration(const long&);    // perform one iteration 
    real* setLabels(char*);
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamset(ofstream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    void sendParameters(int&,real**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const real*);// receive parameter values 
};
#endif
