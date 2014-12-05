#ifndef GEOEXP_H
#define GEOEXP_H

#include "defaultModel.h"

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/geoExp.h,v 1.1 2000/08/31 15:54:57 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  geoExp.h                                                */
/* Contents:     Class definition of the class geoExp                    */
/*               This model is derived from the defaultModel. The difference  */
/*               is that the expected infalation rate is computed by using    */
/*               a geometric distribution on the past tau inflation rates.    */
/*                                                                            */
/* Last Modified: 24.02.1995 (Markus Lohmann)                                 */
/* Modified:      24.02.95 -- sendParameters and receiveParameters added      */
/*                                                                            */
/******************************************************************************/

class geoExp : public defaultModel {
protected:
    qreal expectedInflationRate(const qint64);
    qreal eta;                             // parameter of the geometric lag
				            // distribution
    qreal etaTilda;
    void initialize();
public:
    geoExp();                          // default constructor
    void iteration(const qint64&);            // perform one iteration 
    qreal* setLabels(const QString&);
    void loadParamset(QDataStream&);	    // load parameterset from a file
    void saveParamset(QDataStream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    void sendParameters(int&,qreal**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const qreal*);// receive parameter values 
};
#endif
