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
    qreal a,b;                           /* production function parameters    */
    qreal mu1,mu2;                       /* price adjustment parameters       */
    qreal nu1,nu2;                       /* wage adjustment parameters        */
    qreal alfa0,mqreal0,pg0;              /* initial values                    */
    qreal alfa,mqreal,pgt,mtr,wtr;        /* wage, money stock, price, profit  */
    qreal winf;
    qreal Ls;                            /* maximal aggregate labour supply   */
    qreal h;                             /* utility function parameters       */
    qreal G,tax;                         /* government demand, tax rate       */
    qreal empl;                          /* actual employment                 */
    qreal unemp;                         /* actual unemployment               */ 
    qreal teta1;                         /* actual inflation rate             */ 
    qreal output;                        /* actual output                     */


    qreal virtual_Kemployment();
    qreal labour_demand();
    void diseq_regime(qreal K_emp, qreal l_dem);
    void keynesian_unemployment(qreal K_emp);
    void repressed_inflation (qreal l_dem);
    void classical_unemployment(qreal l_dem);

public:
    milano_bcw();		                    // default constructor
//  virtual ~milano_bcw();		            // destructor

    qreal* setLabels(char*);		    // return a pointer to a variable or
					    // a parameter specified by its name
    void initialize();			    // initialize the model
    void loadParamset(ifstream&);	    // load parameterset from a file
    void saveParamsetWithNames(ofstream&);  // add parametset to XPM printerfile
    void sendStateSpace(int &,const qreal***);// return pointers to all model
					    // variables and the dimension
					    // of the model
    void iteration(const qint64&);            // perform one iteration 



//void printParamset();
//void saveParamset(ofstream&);
//qreal* sendModelVar();
//void sendParameters(int& amount,qreal**);
//void receiveParameters(const qreal*);


};
#endif
