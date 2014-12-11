#ifndef BASEMODEL_H
#define BASEMODEL_H

/******************************************************************************/
/*                                                                            */
/* Module name:  baseModel.h                                                  */
/* Contents:     Class definition of the abstract class baseModel             */
/*               Every other model to be analysed must be derived from this   */
/*               abstract class.                                              */
/*                                                                            */
/* Modified:     07.03.1995 (Markus Lohmann)                                  */
/* Last modified:98/08/21 (Marc Mueller)                                      */
/*                                                                            */
/******************************************************************************/

#include <QTextStream>
#include "../sim.h"
#include "logger.h"

class baseModel
{
protected:
    qint64 length;                            // length of simulation
    uint dimension;	                    // dimension of the system
    qreal xBundle;		            // dummy variables used as default
    qreal yBundle;                           //   settings in an analysis
public:
    baseModel(const uint&);	            // constructor
    virtual ~baseModel()=0;
    virtual qreal* setLabels(const QString&)=0;       // return pointer to a specified
				            //   parameter or variable
    virtual void initialize()=0;            // initialize the model
    virtual void loadParamset(QTextStream&)=0; // load set from a file
    virtual void sendStateSpace(int&,const qreal***)=0;// return the system's
				            //   dimension and pointers to all
				            //   state variables
    virtual void iteration(const qint64&)=0;  // perform one iteration

 //following functions need not to be implemented in any model

    virtual void printParamset();           // print set to the screen
    virtual void saveParamset(QTextStream&);   // write set into a file
    virtual void saveParamsetWithNames(QTextStream&);// add parametset to
					    //   printerfile (only xpm)
    virtual qreal* sendModelVar();           // return pointer to the main
                                            //   model variable
    virtual void receiveParameters(const QList<qreal>&)=0;// receive parameter values
    virtual void sendParameters(int&,qreal**);// write all parameters
                                            //   into an array and return
				            //   the numbers of parameters
    virtual qint64 getLength() const;
};

#endif
