#ifndef COBWEGGPREDICTOR_H
#define COBWEGGPREDICTOR_H

/******************************************************************************/
/*                                                                            */
/* Module name:  cobweb_g_predictor.h                                         */
/* Contents:     Class definition of the class cobweb_g_predictor             */
/*                                                                            */
/* Modified:     28.01.1997 working name was toni.h (Marc Mueller)            */
/* Modified:     07.08.1998 under new name (Marc Mueller)                     */
/* Last Modified:30.01.1997                                                   */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class cobweb_g_predictor : public baseModel {
  protected:
  //Parameters
    real a,b,alpha,w,L0;

    int  L;				// Lag
  //Condition variable
    real *pp0;				// pointer on init price vector
    real *pp;				// pointer on price vector
    real *vv;				// pointer on rates of weights vector
    real v;
    real s;		
  //Model functions
    real hoch(real&,int&);
    real fFunction(real&);
    void dynamics(real&);                        
public:
    cobweb_g_predictor();		 // constructor
    virtual ~cobweb_g_predictor();	 // destructor

    void initialize();			 // initialize the model, define
					 // the initial state
    void loadParamset(ifstream&);	 // load parameterset from a file
    void saveParamset(ofstream&);	 // write parameterset into a file
    void printParamset();		 // print parameterset on the screen
    void saveParamsetWithNames(ofstream&);// add parametset to printerfile
    void sendParameters(int&,real**);    // write all parameters into an array
                                         // and return the numbers of parameters
    void receiveParameters(const real*); // receive parameter values 

    real* setLabels(char*);		 // return a pointer to a variable or
					 // a parameter specified by its name
    real* sendModelVar();		 // return the main model variable
    void sendStateSpace(int &,const real***);// return pointers to all model
					 // variables and the dimension
					 // of the model
    virtual void iteration(const long&); // perform one iteration 
};
#endif
