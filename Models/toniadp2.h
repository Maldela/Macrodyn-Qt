#ifndef TONIADP2_H
#define TONIADP2_H

/******************************************************************************/
/*                                                                            */
/* Module name:  toniadp2.h                                                   */
/* Contents:     Class definition of the class toniadp2                       */
/*                                                                            */
/* Modified:     28.01.1997 (Marc Mueller)                                    */
/* Last Modified:30.01.1997                                                   */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class toniadp2 : public baseModel {
  protected:
  //Parameters
    real gamma,beta,alpha,w,L0;

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
    toniadp2();				// constructor
    virtual ~toniadp2();			// destructor

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
