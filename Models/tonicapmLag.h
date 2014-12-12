#ifndef TONICAPMLAG_H
#define TONICAPMLAG_H

/******************************************************************************/
/*                                                                            */
/* Module name:  tonicapmLag.h                                                */
/* Contents:     Class definition of the class tonicapmLag                    */
/*                                                                            */
/* Modified:     14.08.1997 (Toni Stiefenhofer)                               */
/* Last Modified:14.08.1997                                                   */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"
#include <stdlib.h>

class tonicapmLag : public baseModel {
  protected:
  //Parameters
    qreal a,b,c,d,lambda,sigma,w,L0;

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
    tonicapmLag();				// constructor
    virtual ~tonicapmLag();			// destructor

    void initialize();			 // initialize the model, define
					 // the initial state
    void loadParamset(QTextStream&);	 // load parameterset from a file
    void saveParamset(QTextStream&);	 // write parameterset into a file
    void printParamset();		 // print parameterset on the screen
    void saveParamsetWithNames(QTextStream&);// add parametset to printerfile
    void sendParameters(int&,qreal**);    // write all parameters into an array
                                         // and return the numbers of parameters
    void receiveParameters(const QList<qreal>&); // receive parameter values 

    qreal* setLabels(const QString&);		 // return a pointer to a variable or
					 // a parameter specified by its name
    qreal* sendModelVar();		 // return the main model variable
    void sendStateSpace(int &, QList<qreal *> *);// return pointers to all model
					 // variables and the dimension
					 // of the model
    virtual void iteration(const qint64&); // perform one iteration 
};
#endif
