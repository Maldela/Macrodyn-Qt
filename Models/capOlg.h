#ifndef CAPOLG_H
#define CAPOLG_H
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/capOlg.h,v 1.1 2000/08/31 15:24:04 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  capOlg.h                                                */
/* Contents:     Class definition of the class capOlg                    */
/*                                                                            */
/* Last Modified: 04.07.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class capOlg : public baseModel {
  protected:
    qreal A,B,d;                           // production function parameters
    qreal k0,r0;                           // initial values                
    qreal wt;                              // qreal wage
    qreal n;      
    qreal ymax;                            // qMaximum capacity output
    qreal rho, delta;                      // utility function parameters   
    qint64 tau;                              // length of expectations memory
    qreal rhoTilda;
    qreal rt;		                  // return rate
    qreal kt;                              // capital
    qreal st;
    qreal *rVec;
    qreal indivSave;                       // individual savings
    
    qreal prodFunction(const qreal&);
    virtual qreal expReturnRate(const qint64);                
    qreal savingsFunc(qreal&);                         
    void rShift();
public:
    capOlg();			    // default constructor
    virtual ~capOlg();		    // destructor
    
    void loadParamset(QTextStream&);	    // load parameterset from a file
    void saveParamset(QTextStream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    
    virtual void iteration(const qint64&);    // perform one iteration 
    qreal* setLabels(const QString&);		    // return a pointer to a variable
					    // or a parameter specified by its
					    // name
    void initialize();			    // initialize the model, define
					    // the initial state
    void rInit(qreal*);
    qreal* sendModelVar();		    // return the main model variable
    void sendStateSpace(int &, QList<qreal *> *); // return pointers to all model
					    // variables and the dimension
					    // of the model
    void sendParameters(int&,qreal**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const QList<qreal>&); // receive parameter values 
};

class capOlgAdapt : public capOlg {
protected:
    qreal expectedReturnRate(const qint64);
    qreal oldExpectations;                   // used to save the expected 
				            // return rate for the last period
    qreal eta;                               // weight parameter
public:
    capOlgAdapt();                          // default constructor
    qreal* setLabels(const QString&);
    void iteration(const qint64&);    // perform one iteration 
    void loadParamset(QTextStream&);	    // load parameterset from a file
    void saveParamset(QTextStream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    void sendParameters(int&,qreal**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const QList<qreal>&);// receive parameter values 
};


/******************************************************************************/
/*                                                                            */
/* Class name:   capOlgGeoExp                                                 */
/* Contents:     Class definition of the class capOlgGeoExp                   */
/*               This model is derived from the capOlg. The difference   */
/*               is that the expected infalation rate is computed by using    */
/*               a geometric distribution on the past tau inflation rates.    */
/*                                                                            */
/* Last Modified: 07.09.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

class capOlgGeoExp : public capOlg {
protected:
    qreal expectedReturnRate(const qint64);
    qreal eta;                               // parameter of the geometric lag
				            // distribution
    qreal etaTilda;
    void initialize();
public:
    capOlgGeoExp();                         // default constructor
    void iteration(const qint64&);    // perform one iteration 
    qreal* setLabels(const QString&);
    void loadParamset(QTextStream&);	    // load parameterset from a file
    void saveParamset(QTextStream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    void sendParameters(int&,qreal**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const QList<qreal>&);// receive parameter values 
};
#endif
