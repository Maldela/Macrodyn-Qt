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
    qreal a,b;                           // production function parameters    
    qreal psi1,psi2;                     // price adjustment parameters
    qreal nu1,nu2;                       // wage adjustment parameters      
    qreal omikron1,omikron2;             // interest rate adjustment parameters
    qreal alfa0,mqreal0,pg0,rqreal0,z0;    // initial values                  
    qreal alfa,mqreal,pgt,rqreal,zt,mtr,wtr,rtr;       // wage, money stock, price, profit,interest,interest rate   
    qreal Ls;                            // qMaximal aggregate labour supply
    qreal h;                             // utility function parameters
    qreal G,tax;                         // government demand, tax rate
    qreal empl;                          // actual employment
    qreal output;                        // actual output 
    qreal capital;                       // actual capital stock
    qreal teta;
    qreal epsilon;                       // structural parameter
    qreal f;                             // interest rate adjustment
    void system_1(const qreal&);
    void system_2(const qreal&);   
    void system_4(const qreal&);
    void system_6(const qreal&);
    qreal virtual_s1employment();
    qreal virtual_s4employment(); 
    qreal labour_demand();
    qreal mu_function(const qreal& mu, const qreal& epsilon, 
                     const qreal& b, const qreal& c);
    qreal regfal(int qMax_iter, qreal start_left, qreal start_right,
                qreal c); 
    void diseq_regime(const qreal&,const qreal&,const qreal&);
public:
    milano2();
    virtual ~milano2();
  
    void loadParamset(QTextStream&);	    // load parameterset from a file
    void saveParamset(QTextStream&);	    // write parameterset into a file
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
    void receiveParameters(const QList<qreal>&); // receive parameter values 
};

#endif
