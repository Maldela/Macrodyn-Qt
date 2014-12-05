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
#include "error.h"

class toniadp2 : public baseModel {
  protected:
  //Parameters
    qreal gamma,beta,alpha,w,L0;

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
    toniadp2();				// constructor
    virtual ~toniadp2();			// destructor

    void initialize();			 // initialize the model, define
					 // the initial state
    void loadParamset(QDataStream&);	 // load parameterset from a file
    void saveParamset(QDataStream&);	 // write parameterset into a file
    void printParamset();		 // print parameterset on the screen
    void saveParamsetWithNames(QDataStream&);// add parametset to printerfile
    void sendParameters(int&,qreal**);    // write all parameters into an array
                                         // and return the numbers of parameters
    void receiveParameters(const qreal*); // receive parameter values 

    qreal* setLabels(const QString&);		 // return a pointer to a variable or
					 // a parameter specified by its name
    qreal* sendModelVar();		 // return the main model variable
    void sendStateSpace(int &,const qreal***);// return pointers to all model
					 // variables and the dimension
					 // of the model
    virtual void iteration(const qint64&); // perform one iteration 
};
#endif
