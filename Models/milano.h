#include "baseModel.h"

#define simd length
#ifndef MILANO_H
#define MILANO_H

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/milano.h,v 1.1 2000/08/31 16:06:12 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  milano.h                                                     */
/* Contents:     Class definition of the class milano                         */
/*               This model is taken from the research group in milano        */
/*                                                                            */
/* Last Modified: 16.05.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class milano : public baseModel {
protected:
    int n1;                             // number of producers
    qreal a,b;                           // production function parameters
    qreal mu1,mu2;                       // price adjustment parameters   
    qreal nu1,nu2;                       // wage adjustment parameters      
    qreal alfa0,mqreal0,pg0;                  // initial values                  
    qreal alfa,mqreal,pgt,mtr,wtr;          // wage, money stock, price, profit
    qreal Ls;                            // maximal aggregate labour supply
    qreal h;                             // utility function parameters
    qreal G,tax;                         // government demand, tax rate
    qreal empl;                    // actual employment
    qreal output;                        // actual output 
    void keynesian_unemployment(const qreal&);
    void repressed_inflation(const qreal&);   
    void classical_unemployment(const qreal&);
    qreal virtual_Kemployment();
    qreal labour_demand();
    void diseq_regime(const qreal&,const qreal&);
public:
    milano();
    virtual ~milano();
  
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamset(ofstream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    
    virtual void iteration(const qint64&);    // perform one iteration 
    qreal* setLabels(char*);		    // return a pointer to a variable
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
