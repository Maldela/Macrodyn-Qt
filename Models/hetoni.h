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

    int  L,Ls;				// L = Lag_qint64, Ls = Lag_int

  //Condition variable
    qreal *pp0;				// pointer on init price vector
    qreal *pp;				// pointer on price vector
    qreal *vv;				// pointer on rates of weights vector
    qreal *vvs;				// pointer on rates of int weights vector
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
