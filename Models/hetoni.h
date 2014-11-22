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
    qreal a,b,alpha,w,L0,L1,shr;

    int  L,Ls;				// L = Lag_long, Ls = Lag_short

  //Condition variable
    qreal *pp0;				// pointer on init price vector
    qreal *pp;				// pointer on price vector
    qreal *vv;				// pointer on rates of weights vector
    qreal *vvs;				// pointer on rates of short weights vector
    qreal v;
    qreal s;		
  //Model functions
    qreal hoch(qreal&,int&);
    qreal fFunction(qreal&,qreal&);
    void dynamics(qreal&,qreal&);                        
public:
    hetoni();				// constructor
    virtual ~hetoni();			// destructor

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
