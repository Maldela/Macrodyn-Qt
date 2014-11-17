#include "baseModel.h"

#define simd length
#ifndef MILANO2_INCLUDED
#define MILANO2_INCLUDED

//$Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/milano2.h,v 1.1 2000/08/31 16:06:35 mmeyer Exp $
/******************************************************************************/
/*                                                                            */
/* Module name:  milano2.h                                                    */
/* Contents:     Class definition of the class milano2                        */
/*               This model is taken from the research group in milano        */
/*                                                                            */
/* Last Modified: 16.05.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "baseModel.h"

class milano2 : public baseModel {
protected:
    int n1;                             // number of producers
    real a,b;                           // production function parameters    
    real psi1,psi2;                     // price adjustment parameters
    real nu1,nu2;                       // wage adjustment parameters      
    real omikron1,omikron2;             // interest rate adjustment parameters
    real alfa0,mreal0,pg0,rreal0,z0;    // initial values                  
    real alfa,mreal,pgt,rreal,zt,mtr,wtr,rtr;       // wage, money stock, price, profit,interest,interest rate   
    real Ls;                            // maximal aggregate labour supply
    real h;                             // utility function parameters
    real G,tax;                         // government demand, tax rate
    real empl;                          // actual employment
    real output;                        // actual output 
    real capital;                       // actual capital stock
    real teta;
    real epsilon;                       // structural parameter
    real f;                             // interest rate adjustment
    void system_1(const real&);
    void system_2(const real&);   
    void system_4(const real&);
    void system_6(const real&);
    real virtual_s1employment();
    real virtual_s4employment(); 
    real labour_demand();
    real mu_function(const real& mu, const real& epsilon, 
                     const real& b, const real& c);
    real regfal(int max_iter, real start_left, real start_right,
                real c); 
    void diseq_regime(const real&,const real&,const real&);
public:
    milano2();
    virtual ~milano2();
  
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
