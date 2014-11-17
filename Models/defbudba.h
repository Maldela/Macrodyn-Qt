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
    real A,B;                             // production function parameters
    real gamm,kappa;                      // price adjustment parameters   
    real my,lambda;                       // wage adjustment parameters    
    real w0,p0,m0;                        // initial values                
    real wtreal,mtreal,ptrate,wtrate;     // real wage, real balance       
    real Lmax;                            // maximum labour supply
    real ymax;                            // maximum capacity output
    real rho, delta;                      // utility function parameters   
    int tau;                                // length of expectations memory
    real rhoTilda;
    real g,tax;                           // government demand, tax rate   
    real g0;				  // initial value for g
    real employment;                      // actual employment             
    real beta;
    real *theta;
    real y;                          // actual output
    
    real prodFunction(const real&);
    virtual real expectedInflationRate(const long);                
    void notProd(real&,real&);                        
    void notCom(real&,real&,real&);                         
    void empAndOut(real&,real&);
    void detDiseq(real&,real&,char*);                 
    void wageAndPrice(real&,real&,real&,char*);            
    void dynamics();                        
public:
    defbudba();			    // default constructor
    virtual ~defbudba();		    // destructor
    
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamset(ofstream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    
    virtual void iteration(const long&);    // perform one iteration 
    real* setLabels(char*);		    // return a pointer to a variable
					    // or a parameter specified by its
					    // name
    void initialize();			    // initialize the model, define
					    // the initial state
    void thetaInit(real*);
    void getTheta(real*);
    void setTheta(real*);
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
