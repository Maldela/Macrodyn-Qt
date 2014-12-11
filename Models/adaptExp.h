#ifndef ADAPTEXP_H
#define ADAPTEXP_H

#include "defaultModel.h"
//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/adaptExp.h,v 1.1 2000/08/31 15:22:45 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  adaptExp.h                                              */
/* Contents:     Class definition of the class adaptExp                  */
/*               This model is derived from the defaultModel. The difference  */
/*               is that the expected infalation rate is computed by using    */
/*               adaptive expectations.                                       */
/*                                                                            */
/* Last Modified: 24.02.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                                                                            */
/******************************************************************************/

class adaptExp : public defaultModel {
protected:
    qreal expectedInflationRate(const qint64);
    qreal oldExpectations;                 // used to save the expected
				            // inflation rate for the last
				            // period
    qreal eta;                             // weight parameter
public:
    adaptExp();                        // default constructor
    void iteration(const qint64 &);           // perform one iteration
    qreal* setLabels(const QString&);
    void loadParamset(QTextStream&);	    // load parameterset from a file
    void saveParamset(QTextStream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    void sendParameters(int&,qreal**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const QList<qreal>);// receive parameter values
};
#endif
