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
    toni2();				// constructor
    virtual ~toni2();			// destructor

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
