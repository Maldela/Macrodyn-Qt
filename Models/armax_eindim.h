///////////////////////////////////////////////////////////////////////////////
//
// $Header: armax_eindim.h Dec 20 METDST 2000
//
// Module name:		armax_eindim.h
// Contents:		Class definition of the class armax_eindim
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _armax_eindim_INCLUDED
#define _armax_eindim_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "../matrix.h"
#include "distribution.h"
#include "../matrix_neu.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		armax_eindim
// Derived from:	baseModel
// Purpose:		defines the armax_eindim
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

class armax_eindim : public baseModel {

protected:
	static const int L=2, K=2, R=0, G=0, H=0;
// L,K,R are the dimensions for y_old, u_old and v_old respectively
// where L,K in {1,2,...} and R in {0,1,...}
// G,H in {0,1} are switches for w_t and e respectively
// y_t = Sum_1_L{a_i*y_t-i} + Sum_1_K{b_i*u_t-i} + v_t + Sum_1_R{c_i*v_t-i} +
// 	   + d*w_t + e*1


	distribution *rand_dis;	//needed for pseudo-random number
	real rand_rec;          //generates a rectangular unit variate
	
	real y_t;			//value of y in period t
	real y_old_ini[L];  //stores values of sim-file
	real y_old[L];		//y_old[0] = y_t-1, ..., y_old[L-1] = y_t-L
	real u_t;			//value of u in period t
	real u_old_ini[K];  //stores values of sim-file
	real u_old[K];		//u_old[0] = u_t-1, ..., u_old[K-1] = u_t-K	
	real v_t;			//unobservable interference (error) --> random variable
	real v_old_ini[R];  //stores values of sim-file
	real v_old[R];		//v_old[0] = v_t-1, ..., v_old[R-1] = v_t-R	
	real a[L];			//true parameter for a loaded from sim-file
	real b[K];			//true parameter for b loaded from sim-file
	real c[R];			//true parameter for c loaded from sim-file
    real d;             //true parameter for d loaded from sim-file
    real e;             //true parameter for e loaded from sim-file

        		
	real a_e[L];		//estimateted parameter for a
	real b_e[K];		//estimateted parameter for b
	real c_e[R];		//estimateted parameter for c
	real d_e;			//estimateted parameter for d
	real e_e;			//estimateted parameter for e
	
	
// estimation is done via inversion:
// u_t-1 = b_1^(-1)*y_t - Sum_1_L{b_1^(-1)*a_i*y_t-i} - Sum_2_L{b_1^(-1)*b_i*u_t-i} -
// 		   - Sum_1_R{b_1^(-1)*c_i*v_t-i} - b_1^(-1)*v_t - b_1^(-1)*d*w_t - b_1^(-1)*e
// estimate via ELS the new coefficients as,b,c,d,e	
	
	real as_e[L+1];		//estimateted parameter for as
						//as_e[0] = 1/b[0] (=b_1^(-1))
						//as_e[1] = 1/b[0] * a[0]
						// ...
	real bs_e[K-1];		//estimateted parameter for bs
	real cs_e[R];		//estimateted parameter for cs
	real ds_e;		//estimateted parameter for ds
	real es_e;		//estimateted parameter for es
	
	real error_as_e[L+1];	// as_e[] - as[]
	real error_bs_e[K-1];
	real error_cs_e[R];	
 	real error_ds_e;
 	real error_es_e;

 	
 	
 	real sum_u;
 	real sum_y;
 	real sum_uy;
 	real norm_u;
 	real norm_y;
 	real sum_u_buf;
 	real sum_y_buf;
 	real sigma_eps;
 	real eps;
	real lambda_tm1;			
	real eps_t;		
	real s_t;
	real eta_t;
	real var1, var2;
	real sum_var1, sum_var2;
	real dif_var,dif;
	
//used for ELS:	
	real temp1;
	matrix_neu* Temp11;			//(1x1)-matrix
	matrix_neu* TempLKRGH_1; 	//(LKRGH x 1)-matrix
	matrix_neu* TempLKRGH_1_2;   //another (LKRGH x 1)-matrix
	matrix_neu* TempLKRGH_LKRGH; //...
	matrix_neu* TempLKRGH_LKRGH_2;
	matrix_neu* Temp1_LKRGH;
	matrix_neu* x_tm1;
	matrix_neu* Theta;
	matrix_neu* P_tm1;
	matrix_neu* P_t;
	matrix_neu* P_t_inv;
	matrixFn* point;	// needed to use functions of matrixFn-class
	
	
	
public:
    armax_eindim();				// constructor
    virtual ~armax_eindim();		// destructor
    void loadParamset(ifstream&);	// load parameterset from a file
    void saveParamset(ofstream&);
    void printParamset();
    void sendParameters(int &,real**);
    real* sendModelVar(void);
    void receiveParameters(const real*);
    real* setLabels(char*);		// get a variable by a name
    void initialize(void);		// initialize the model
    void iteration(const long&);	// perform one iteration 	
    void sendStateSpace(int &,const real***);

};

#endif //  _armax_eindim_INCLUDED
