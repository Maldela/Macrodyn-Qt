#ifndef TONIRV_H
#define TONIRV_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/toniRV.h,v 1.1 2000/08/31 16:10:47 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  toniRV.h                                                       */
/* Contents:     Class definition of the class toniRV                           */
/*                                                                            */
/* Modified:     28.01.1997 (Marc Mueller)                                    */
/* Last Modified:30.01.1997                                                   */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class toniRV : public baseModel {
  protected:
  //Parameters
    qreal a,b,alpha,w,L0;

    int  L;				// Lag
  //Condition variable
    qreal *pp0;				// pointer on init price vector
    qreal *pp;				// pointer on price vector
    qreal *vv;				// pointer on rates of weights vector
    qreal v;
    qreal s;		
  //Model functions
    qreal hoch(qreal&,int&);
    qreal fFunction(qreal&);
    void dynamics(qreal&);                        
public:
    toniRV();				// constructor
    virtual ~toniRV();			// destructor

    void initialize();			 // initialize the model, define
					 // the initial state
    void loadParamset(ifstream&);	 // load parameterset from a file
    void saveParamset(ofstream&);	 // write parameterset into a file
    void printParamset();		 // print parameterset on the screen
    void saveParamsetWithNames(ofstream&);// add parametset to printerfile
    void sendParameters(int&,qreal**);    // write all parameters into an array
                                         // and return the numbers of parameters
    void receiveParameters(const qreal*); // receive parameter values 

    qreal* setLabels(char*);		 // return a pointer to a variable or
					 // a parameter specified by its name
    qreal* sendModelVar();		 // return the main model variable
    void sendStateSpace(int &,const qreal***);// return pointers to all model
					 // variables and the dimension
					 // of the model
    virtual void iteration(const long&); // perform one iteration 
};
#endif
