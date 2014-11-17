#ifndef MILANO_BCW_H
#define MILANO_BCW_H
/****************************************************************************/
/*                                                                          */
/* Module name:  bcw.h                                                      */
/* Contents:     simlinph.h                                                 */
/*                                                                          */
/* Modified:     98/08/14 Marc Mueller                                      */
/*               99/07/16 Gerd, Luca                                        */
/****************************************************************************/

#include "baseModel.h"

class milano_bcw : public baseModel {
  protected:
    int n1;                             /* number of producers               */
    real a,b;                           /* production function parameters    */
    real mu1,mu2;                       /* price adjustment parameters       */
    real nu1,nu2;                       /* wage adjustment parameters        */
    real alfa0,mreal0,pg0;              /* initial values                    */
    real alfa,mreal,pgt,mtr,wtr;        /* wage, money stock, price, profit  */
    real winf;
    real Ls;                            /* maximal aggregate labour supply   */
    real h;                             /* utility function parameters       */
    real G,tax;                         /* government demand, tax rate       */
    real empl;                          /* actual employment                 */
    real unemp;                         /* actual unemployment               */ 
    real teta1;                         /* actual inflation rate             */ 
    real output;                        /* actual output                     */


    real virtual_Kemployment();
    real labour_demand();
    void diseq_regime(real K_emp, real l_dem);
    void keynesian_unemployment(real K_emp);
    void repressed_inflation (real l_dem);
    void classical_unemployment(real l_dem);

public:
    milano_bcw();		                    // default constructor
//  virtual ~milano_bcw();		            // destructor

    real* setLabels(char*);		    // return a pointer to a variable or
					    // a parameter specified by its name
    void initialize();			    // initialize the model
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamsetWithNames(ofstream&);  // add parametset to XPM printerfile
    void sendStateSpace(int &,const real***);// return pointers to all model
					    // variables and the dimension
					    // of the model
    void iteration(const long&);            // perform one iteration 



//void printParamset();
//void saveParamset(ofstream&);
//real* sendModelVar();
//void sendParameters(int& amount,real**);
//void receiveParameters(const real*);


};
#endif
