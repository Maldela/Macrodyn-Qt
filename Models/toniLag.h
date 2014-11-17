#ifndef TONILAG_H
#define TONILAG_H

/******************************************************************************/
/*                                                                            */
/* Module name:  toniLag.h                                                   */
/* Contents:     Class definition of the class toniLag                        */
/*                                                                            */
/* Modified:     28.01.1997 (Marc Mueller)                                    */
/* Last Modified:10.08.1997 (Toni Stiefenhofer)                               */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"
#include <stdlib.h>

class toniLag : public baseModel {
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
    toniLag();				// constructor
    virtual ~toniLag();			// destructor

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
