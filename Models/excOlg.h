#ifndef EXCOLGMODEL_H
#define EXCOLGMODEL_H
//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/excOlg.h,v 1.1 2000/08/31 15:54:18 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  excOlg.h                                                */
/* Contents:     Class definition of the classes excOlg, excOlgAdapt and */
/*               excOlgGeoExp                                                 */
/*                                                                            */
/* Last Modified: 07.09.1995 (Markus Lohmann)                                 */
/* Modified:      07.09.95 excOlgAdapt and excOlgGeoExp added (M. Lohmann)    */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class excOlg : public baseModel {
  protected:
    qreal theta0;                          // initial value                
    qreal rho, delta;                      // utility function parameters   
    int tau;                              // length of expectations memory
    qreal rhoTilda;
    qreal thetat;		          // inflation rate
    qreal oldThetat;		          // old inflation rate
    qreal oldThetaE;
    qreal savOld;
    qreal savYoung;
    qreal *thetaVec;
    
    virtual qreal expInflRateYoung(const qint64);                
    virtual qreal expInflRateOld(const qint64);                
    qreal savingsFuncYoung(qreal&);                         
    qreal savingsFuncOld(qreal&);                         
    void thetaShift();
public:
    excOlg();			    // default constructor
    virtual ~excOlg();		    // destructor
    
    void loadParamset(QDataStream&);	    // load parameterset from a file
    void saveParamset(QDataStream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    
    virtual void iteration(const qint64&);    // perform one iteration 
    qreal* setLabels(const QString&);		    // return a pointer to a variable
					    // or a parameter specified by its
					    // name
    void initialize();			    // initialize the model, define
					    // the initial state
    void thetaInit(qreal*);
    void getTheta(qreal*);
    void setTheta(qreal*);
    qreal* sendModelVar();		    // return the main model variable
    void sendStateSpace(int &,const qreal***); // return pointers to all model
					    // variables and the dimension
					    // of the model
    void sendParameters(int&,qreal**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const qreal*); // receive parameter values 
};


class excOlgAdapt : public excOlg {
protected:
    qreal expInflRateYoung(const qint64);
    qreal expInflRateOld(const qint64);
    qreal oldExpectationsYoung;              // used to save the expected 
    qreal oldExpectationsOld;                // return rate for the last period
    qreal etaYoung;                          // weight parameter young
    qreal etaOld;                            // weight parameter old
public:
    excOlgAdapt();                          // default constructor
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


/******************************************************************************/
/*                                                                            */
/* Class name:   excOlgGeoExp                                                 */
/* Contents:     Class definition of the class excOlgGeoExp                   */
/*               This model is derived from the excOlg. The difference   */
/*               is that the expected infalation rate is computed by using    */
/*               a geometric distribution on the past tau inflation rates.    */
/*                                                                            */
/* Last Modified: 07.09.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

class excOlgGeoExp : public excOlg {
protected:
    qreal expInflRateYoung(const qint64);
    qreal expInflRateOld(const qint64);
    qreal etaYoung;                          // parameter of the geometric lag
    qreal etaOld;    		            // distribution
    qreal etaTildaYoung;
    qreal etaTildaOld;
    void initialize();
public:
    excOlgGeoExp();                         // default constructor
    void iteration(const qint64&);            // perform one iteration 
    qreal* setLabels(const QString&);
    void loadParamset(QDataStream&);	    // load parameterset from a file
    void saveParamset(QDataStream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    void sendParameters(int&,qreal**);       // write all parameters
				// into an array and return the numbers
				// of parameters
    void receiveParameters(const qreal*);// receive parameter values 
};
#endif
