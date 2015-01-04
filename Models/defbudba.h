#ifndef DEFBUDBA_H
#define DEFBUDBA_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/defbudba.h,v 1.1 2000/08/31 15:25:19 mmeyer Exp $

/******************************************************************************/
/*                                                                            */
/* Module name:  defaultModel.h                                               */
/* Contents:     Class definition of the class defaultModel                   */
/*               This model is taken from the discussion paper No. 288 of the */
/*               university of bielefeld: "Dynamic Complexity in a Keynesian  */
/*               Macroeconomic Model" by Volker Boehm, Markus Lohmann and     */
/*               Hans-Walter Lorenz                                           */
/*                                                                            */
/* Last Modified: 09.11.1994 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class defbudba : public baseModel {
  protected:
    qreal A,B;                             // production function parameters
    qreal gamm,kappa;                      // price adjustment parameters   
    qreal my,lambda;                       // wage adjustment parameters    
    qreal w0,p0,m0;                        // initial values                
    qreal wtreal,mtreal,ptrate,wtrate;     // qreal wage, qreal balance       
    qreal Lmax;                            // qMaximum labour supply
    qreal ymax;                            // qMaximum capacity output
    qreal rho, delta;                      // utility function parameters   
    qint64 tau;                                // length of expectations memory
    qreal rhoTilda;
    qreal g,tax;                           // government demand, tax rate   
    qreal g0;				  // initial value for g
    qreal employment;                      // actual employment             
    qreal beta;
    qreal *theta;
    qreal y;                          // actual output
    
    qreal prodFunction(const qreal&);
    virtual qreal expectedInflationRate(const qint64);                
    void notProd(qreal&,qreal&);                        
    void notCom(qreal&,qreal&,qreal&);                         
    void empAndOut(qreal&,qreal&);
    void detDiseq(qreal&,qreal&,char*);                 
    void wageAndPrice(qreal&,qreal&,qreal&,char*);            
    void dynamics();                        
public:
    defbudba();			    // default constructor
    virtual ~defbudba();		    // destructor
    
    void loadParamset(QTextStream&);	    // load parameterset from a file
    void saveParamset(QTextStream&);	    // write parameterset into a file
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
    void sendStateSpace(int &, QList<qreal *> *); // return pointers to all model
					    // variables and the dimension
					    // of the model
    void sendParameters(int&,qreal**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const QList<qreal>&); // receive parameter values
};

#endif
