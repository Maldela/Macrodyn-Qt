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
    real a,b;                           // production function parameters    
    real psi1,psi2;                     // price adjustment parameters
    real nu1,nu2;                       // wage adjustment parameters      
    real alfa0,mreal0,pg0,z0;    // initial values                  
    real alfa,mreal,pgt,rreal,zt,mtr,wtr,rtr;       // wage, money stock,profit,interest,interest rate   
    real Ls;                            // maximal aggregate labour supply
    real h;                             // utility function parameters
    real G,tax;                         // government demand, tax rate
    real empl;                          // actual employment
    real output;                        // actual output 
    real capital;                       // actual capital stock
    real teta;
	
    void system_3(const real&);
    void system_G6(const real&);   
    void system_8(const real&);
    real Zins_1();
    real Zins_2(); 
    real Zins_3(const real&, const real&);
    real zins_3_function(const real& r);
    void diseq_regime(const real&,const real&,const real&);
public:
    milano3();
    virtual ~milano3();
  
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












