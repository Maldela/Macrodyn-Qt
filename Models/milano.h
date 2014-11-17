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
    real a,b;                           // production function parameters
    real mu1,mu2;                       // price adjustment parameters   
    real nu1,nu2;                       // wage adjustment parameters      
    real alfa0,mreal0,pg0;                  // initial values                  
    real alfa,mreal,pgt,mtr,wtr;          // wage, money stock, price, profit
    real Ls;                            // maximal aggregate labour supply
    real h;                             // utility function parameters
    real G,tax;                         // government demand, tax rate
    real empl;                    // actual employment
    real output;                        // actual output 
    void keynesian_unemployment(const real&);
    void repressed_inflation(const real&);   
    void classical_unemployment(const real&);
    real virtual_Kemployment();
    real labour_demand();
    void diseq_regime(const real&,const real&);
public:
    milano();
    virtual ~milano();
  
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamset(ofstream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    
    virtual void iteration(const long&);    // perform one iteration 
    real* setLabels(char*);		    // return a pointer to a variable
					    // or a parameter specified by its
					    // name
    void initialize();			    // initialize the model, define
					    // the initial state
    real* sendModelVar();		    // return the main model variable
    void sendStateSpace(int &,const real***); // return pointers to all model
					    // variables and the dimension
					    // of the model
    void sendParameters(int&,real**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const real*); // receive parameter values 
};

#endif
