///////////////////////////////////////////////////////////////////////////////
//
// $Header: armax.h Dec 20 METDST 2000
//
// Module name:		armax.h
// Contents:		Class definition of the class armax
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _armax_INCLUDED
#define _armax_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
#include "../matrix.h"
#include "distribution.h"
#include "../matrix_neu.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		armax
// Derived from:	baseModel
// Purpose:		defines the armax
//
// Author:		    mhoffman
// Last modified:	Feb 27 METDST 2001
// By:				mhoffman			
//
///////////////////////////////////////////////////////////////////////////////

class armax : public baseModel {

protected:
	static const int M=2, N1=2, N2=2, N3=0, L=2, K=2, R=0, G=0, H=0;
// L,K,R are the lags for y_old, u_old and v_old respectively
// where L,K in {1,2,...} and R in {0,1,...}
// G,H in {0,1} are switches for w_t and e respectively
// y_t = Sum_1_L{a_i*y_t-i} + Sum_1_K{b_i*u_t-i} + v_t + Sum_1_R{c_i*v_t-i} +
// 	   + d*w_t + e*1


	distribution *rand_dis;	//needed for pseudo-random number
	real rand_rec;          //generates a rectangular unit variate
	
	matrix_neu* y_t;			//value of y in period t
	matrix_neu* y_old_ini[L];  //stores values of sim-file
	matrix_neu* y_old[L];		//y_old[0] = y_t-1, ..., y_old[L-1] = y_t-L
	matrix_neu* u_t;			//value of u in period t
	matrix_neu* u_old_ini[K];  //stores values of sim-file
	matrix_neu* u_old[K];		//u_old[0] = u_t-1, ..., u_old[K-1] = u_t-K	
	matrix_neu* v_t;			//unobservable interference (error) --> random variable
	matrix_neu* v_old_ini[R];  //stores values of sim-file
	matrix_neu* v_old[R];		//v_old[0] = v_t-1, ..., v_old[R-1] = v_t-R	
	matrix_neu* w_t;
	matrix_neu* A[L];			//true parameter for a loaded from sim-file
	matrix_neu* B[K];			//true parameter for b loaded from sim-file
	matrix_neu* C[R];			//true parameter for c loaded from sim-file
    matrix_neu* D;             //true parameter for d loaded from sim-file
    matrix_neu* E;             //true parameter for e loaded from sim-file
    real u;
    real y;
    real norm_u;               //euklidische Norm² von u
    real norm_y;               //        "            y
    real sum_u;                //1/n * SUM (norm_u)
    real sum_y;
    real sum_uy;
    real normA1, normA2, normA3;
    real normB1;


        		
	matrix_neu* A_e[L];		//estimateted parameter for a
	matrix_neu* B_e[K];		//estimateted parameter for b
	matrix_neu* C_e[R];		//estimateted parameter for c
	matrix_neu* D_e;			//estimateted parameter for d
	matrix_neu* E_e;			//estimateted parameter for e
	
	
// estimation is done via inversion:
// u_t-1 = b_1^(-1)*y_t - Sum_1_L{b_1^(-1)*a_i*y_t-i} - Sum_2_L{b_1^(-1)*b_i*u_t-i} -
// 		   - Sum_1_R{b_1^(-1)*c_i*v_t-i} - b_1^(-1)*v_t - b_1^(-1)*d*w_t - b_1^(-1)*e
// estimate via ELS the new coefficients as,b,c,d,e	
	
	matrix_neu* As_e[L+1];		//estimateted parameter for as
						//as_e[0] = 1/b[0] (=b_1^(-1))
						//as_e[1] = 1/b[0] * a[0]
						// ...
	matrix_neu* Bs_e[K-1];		//estimateted parameter for bs
	matrix_neu* Cs_e[R];		//estimateted parameter for cs
	matrix_neu* Ds_e;		//estimateted parameter for ds
	matrix_neu* Es_e;		//estimateted parameter for es
	

	real lambda_tm1;			
	real eps_t;		
	real eps;      		
	real sigma_eps;
	real s_t;
	real var1;
	real var2;
	real sum_var1;
	real sum_var2;
	real dif_var;
	matrix_neu* dif;
	
//used for ELS:	
	
	
	matrix_neu* difA;
	matrix_neu* difB;
	
	matrix_neu* eta_t;		//dither
	matrix_neu* B_1_inv;
	matrix_neu* TempLKRGH_1; 	//(LKRGH x 1)-matrix
	matrix_neu* TempLKRGH_1_2;   //another (LKRGH x 1)-matrix
	matrix_neu* TempLKRGH_LKRGH; //...
	matrix_neu* TempLKRGH_LKRGH_2;
	matrix_neu* Temp1_LKRGH;
	matrix_neu* x_tm1;	//wird ein Zeilenvektor !!
	matrix_neu* Theta;
	matrix_neu* P_tm1;
	matrix_neu* P_t;
	
	
	
public:
    armax();				// constructor
    virtual ~armax();		// destructor
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

#endif //  _armax_INCLUDED
