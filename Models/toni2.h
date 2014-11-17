#ifndef TONI2_H
#define TONI2_H
//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/toni2.h,v 1.1 2000/08/31 16:10:09 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  toni2.h                                                       */
/* Contents:     Class definition of the class toni2                           */
/*                                                                            */
/* Modified:     28.01.1997 (Marc Mueller)                                    */
/* Last Modified:30.01.1997                                                   */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class toni2 : public baseModel {
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
    toni2();				// constructor
    virtual ~toni2();			// destructor

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
