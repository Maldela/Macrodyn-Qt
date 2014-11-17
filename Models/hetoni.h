#ifndef HETONI_H
#define HETONI_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/hetoni.h,v 1.1 2000/08/31 15:57:25 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  hetoni.h                                                     */
/* Contents:     Class definition of the class hetoni                         */
/*                                                                            */
/* Modified:     20.04.1997 (Marc Mueller, Toni Stiefenhofer)                 */
/* Last Modified:20.04.1997                                                   */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class hetoni : public baseModel {
  protected:
  //Parameters
    real a,b,alpha,w,L0,L1,shr;

    int  L,Ls;				// L = Lag_long, Ls = Lag_short

  //Condition variable
    real *pp0;				// pointer on init price vector
    real *pp;				// pointer on price vector
    real *vv;				// pointer on rates of weights vector
    real *vvs;				// pointer on rates of short weights vector
    real v;
    real s;		
  //Model functions
    real hoch(real&,int&);
    real fFunction(real&,real&);
    void dynamics(real&,real&);                        
public:
    hetoni();				// constructor
    virtual ~hetoni();			// destructor

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
