#ifndef NICOLE_H
#define NICOLE_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/nicole.h,v 1.1 2000/08/31 16:07:36 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  nicole.h                                                     */
/* Contents:     Class definition of the class nicole                         */
/*                                                                            */
/* Modified:     16.04.1997 (Marc Mueller)                                    */
/* Last Modified:21.04.1997                                                   */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"
#include "rand_var.h"

class nicole : public baseModel {
  protected:
    char zvar_expr[256];	// expression which defines a random variable
    rand_var * zvar;		// a random variable    
  //Parameters
    real pi,dl,dh,d0,e,alpha,xhat,theta,R;
  //Condition variable
    real p;				//price vector
    real d;				//stochastic
  //Model functions
    void getD();                        
    void dynamics();                        
public:
    nicole();				// constructor
    virtual ~nicole();			// destructor
    void initialize();			 // initialize the model
    void loadParamset(ifstream&);	 // load parameterset from a file
    void saveParamset(ofstream&);	 // write parameterset into a file
    void printParamset();		 // print parameterset on the screen
    void saveParamsetWithNames(ofstream&);// add parametset to printerfile
    real* setLabels(char*);		 // return a pointer to a variable
    virtual void iteration(const long&); // perform one iteration 
    real* sendModelVar();		 // obsolete and unused
    void sendStateSpace(int &,const real***);// obsolete
    void sendParameters(int&,real**); 	 // unused
    void receiveParameters(const real*); // unused 
};
#endif
