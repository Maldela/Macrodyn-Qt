#include "baseModel.h"

#define simd length
#ifndef MILANO3_H
#define MILANO3_H

//$Header: milano3.h,v 1.4 98/03/23 15:56:26 jnikutta Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  milano3.h                                                     */
/* Contents:     Class definition of the class milano                         */
/*               This model is taken from the research group in milano        */
/*                                                                            */
/* Last Modified:                                                            */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class milano3 : public baseModel {
protected:
    int n1;                             // number of producers
    qreal a,b;                           // production function parameters    
    qreal psi1,psi2;                     // price adjustment parameters
    qreal nu1,nu2;                       // wage adjustment parameters      
    qreal alfa0,mqreal0,pg0,z0;    // initial values                  
    qreal alfa,mqreal,pgt,rqreal,zt,mtr,wtr,rtr;       // wage, money stock,profit,interest,interest rate   
    qreal Ls;                            // maximal aggregate labour supply
    qreal h;                             // utility function parameters
    qreal G,tax;                         // government demand, tax rate
    qreal empl;                          // actual employment
    qreal output;                        // actual output 
    qreal capital;                       // actual capital stock
    qreal teta;
	
    void system_3(const qreal&);
    void system_G6(const qreal&);   
    void system_8(const qreal&);
    qreal Zins_1();
    qreal Zins_2(); 
    qreal Zins_3(const qreal&, const qreal&);
    qreal zins_3_function(const qreal& r);
    void diseq_regime(const qreal&,const qreal&,const qreal&);
public:
    milano3();
    virtual ~milano3();
  
    void loadParamset(QDataStream&);	    // load parameterset from a file
    void saveParamset(QDataStream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    
    virtual void iteration(const qint64&);    // perform one iteration 
    qreal* setLabels(const QString&);		    // return a pointer to a variable
					    // or a parameter specified by its
					    // name
    void initialize();			    // initialize the model, define
					    // the initial state
    qreal* sendModelVar();		    // return the main model variable
    void sendStateSpace(int &,const qreal***); // return pointers to all model
					    // variables and the dimension
					    // of the model
    void sendParameters(int&,qreal**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const qreal*); // receive parameter values 
};

#endif












