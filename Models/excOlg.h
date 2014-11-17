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
    real theta0;                          // initial value                
    real rho, delta;                      // utility function parameters   
    int tau;                              // length of expectations memory
    real rhoTilda;
    real thetat;		          // inflation rate
    real oldThetat;		          // old inflation rate
    real oldThetaE;
    real savOld;
    real savYoung;
    real *thetaVec;
    
    virtual real expInflRateYoung(const long);                
    virtual real expInflRateOld(const long);                
    real savingsFuncYoung(real&);                         
    real savingsFuncOld(real&);                         
    void thetaShift();
public:
    excOlg();			    // default constructor
    virtual ~excOlg();		    // destructor
    
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


class excOlgAdapt : public excOlg {
protected:
    real expInflRateYoung(const long);
    real expInflRateOld(const long);
    real oldExpectationsYoung;              // used to save the expected 
    real oldExpectationsOld;                // return rate for the last period
    real etaYoung;                          // weight parameter young
    real etaOld;                            // weight parameter old
public:
    excOlgAdapt();                          // default constructor
    void iteration(const long&);            // perform one iteration 
    real* setLabels(char*);
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamset(ofstream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    void sendParameters(int&,real**); // write all parameters
                                // into an array and return the numbers
				// of parameters
    void receiveParameters(const real*);// receive parameter values 
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
    real expInflRateYoung(const long);
    real expInflRateOld(const long);
    real etaYoung;                          // parameter of the geometric lag
    real etaOld;    		            // distribution
    real etaTildaYoung;
    real etaTildaOld;
    void initialize();
public:
    excOlgGeoExp();                         // default constructor
    void iteration(const long&);            // perform one iteration 
    real* setLabels(char*);
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamset(ofstream&);	    // write parameterset into a file
    void printParamset();		    // print parameterset on the screen
    void sendParameters(int&,real**);       // write all parameters
				// into an array and return the numbers
				// of parameters
    void receiveParameters(const real*);// receive parameter values 
};
#endif
