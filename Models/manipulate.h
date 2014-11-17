///////////////////////////////////////////////////////////////////////////////
//
// Module name:		manipulate.h
// Contents:		Class definition of the class manipulate
//			
// Author:			Marc Mueller
// Last modified:	Tue Sep 26 14:29:39 METDST 2000	
// By:				Mathias Hoffmann & Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MANIPULATE_H
#define MANIPULATE_H

#include "baseModel.h"
#include "rand_var.h"
#include "distribution.h"
#include "markov_chain.h"
#include "../strnchr.h"
#include "../matrix.h"
#include<stdlib.h>

class manipulate : public baseModel {

protected:
	baseModel* model;
	char modelTyp[40];
	int howMany;
	int* manipulateTag;
	char varname[40];
	enum manipulateMode {	stoch_uni=10, stoch_markov=11, stoch_ar=12,
				errcor=50,
				ma=100, ma_k, ma_k_a,
				rls0=98, rls1=99, 
				ols=110, rls=120, rls1d, els=130, sg=140,
				statistics=200,
				quantiles=210 };
	// case stoch_uni
		void F_stoch_uni();
   	    real * zvar_ptr;		// pointer to the stochastic parameter
		rand_var * zvar;		// ptr random function   
		char zvar_expr[256];
		real *randname;		    // Modellvariable in die die gezogene Zufallsvariable eingetragen wird

	// case stoch_markov
		void F_stoch_markov();
		markov_chain * markov;	// Zeiger auf Markovkette
		int statesNum;		// Anzahl der Zustaende
		char m_state[256];	// Zustaende als Zeichenkette fuer
		char m_matrix[1024];	// Uebergangsmatrix als Zeichenkette
		real states[32];	// Zustaende als real-Feld angeordnent
		real umatrix[32][32];	// Uebergangsmatrix als real-Matrix
		real zetamin;
		real zetamax;
		real* expname;		// Modellvariable in die der Erwartungswert eingetragen wird
		real* variancename; 		// Modellvariable in die die Varianz eingetragen wird
		//real* randname;		// Modellvariable in die die gezogene Zufallsvariable eingetragen wird

	// case stoch_markov
		void F_stoch_ar();
		//rand_var * zvar;	// ptr random function   
		real gamma;
		//real zetamin;
		//real zetamax;
		//real* expname;		// Modellvariable in die der Erwartungswert eingetragen wird
		//real* variancename;		// Modellvariable in die die Varianz eingetragen wird
		//real* randname;		// Modellvariable in die die gezogene Zufallsvariable eingetragen wird

	// case errcor  forcast a new price with weighted last error  50 num price preiceForecast alpha
		// Period t-1 is just done: pF(t)=p(t-1)+alpha*(pF(t-2)-p(t-1))
		void F_errcor();
		struct struct_errcor {real* price;real* forecast;real forecastOld;real forecastOld2;real alpha;};
		struct_errcor* errcor_ptr;
		int errcor_num;

	// case ma total average  100 1 q p
		void F_ma(const long int &);
		struct struct_ma {real* mname;real* vname;real vn;};
		struct_ma* ma_ptr;
		int ma_num;
	// case ma_k	101 1 q 5 p
		void F_ma_k();
		struct struct_ma_k {real* mname;real* vname;int k;real* vn_k;};
		struct_ma_k* ma_k_ptr;
		int ma_k_num;
	// case ma_k_a	102 1 q 5 p	// vname=a/k*sum_vn_k//
		void F_ma_k_a();
		struct struct_ma_k_a {real* mname;real* vname;int k;real* vn_k;real a;};
		struct_ma_k_a* ma_k_a_ptr;
		int ma_k_a_num;

	// case rls1 98 scalar inputvector outputvector
		// 98 g_0 p_tm1 p_e_tp1
		// from Jan
		void F_rls0();
		real g_0;	// init value
		real g_tm1;	//
  		real* p_tm1;	// pointer to price of last period
 		real* p_e_tp1;	// vector to set in forecast
                real beta_tm1;	// estimator
		real p_tm2;	// save old price for next period
	// case rls1 99 g_0 theta_eNew theta_e theta
		//	   99 0.5 thetaEtp1 thetaEt thetatm1
		// from Model Martin
		void F_rls1();
		//real g_tm1; // already defined in rls0
		real* theta_tm1;
		real* theta_e_tp1;
		real* theta_e_t;
	// case ols	110 val yPointer y_num  varphi_num  varphiPointer_i vpP_length_i vpP_lag_i
		//		110  0 thetaEtp1 1  2  theta1 1 1 thetaEt 1 1
		olsClass* OLS;
		void F_ols();
		int olsStart;
		int forecasttype;
	// case rls	120 val yPointer y_num  varphi_num  varphiPointer_i vpP_length_i vpP_lag_i Forecast
		//	120 1000000 q 1  2  p 1 3 d 1 1 F1
		//          val = initvalue of alpha
		// 	    q 1 = forecast portfolio q with one asset
		//	    p 1 3 = input is old 3 prices of one asset
		//          F1 = do forecast for the next period
		//       or F2 = do forecast for the period after the next period
		rlsClass* RLS;
		void F_rls();
	// case rls1d	121 helpvar expectedPrice price Forecast
		//	121 helpvar q p F2
		real oldp,P,theta;
		real* q;
		real* p;
		void F_rls1d();
	// case rls	130 val yPointer y_num  varphi_num  varphiPointer_i vpP_length_i vpP_lag_i  mum  Forecast
		//	130 100000 q 1  2  p 1 3  d 1 1  2  F1
		//          val = initvalue of alpha
		// 	    q 1 = forecast portfolio q with one asset
		//	    p 1 3 = input is old 3 prices of one asset
		//          num 2 = lag of estimate error correction
		//          F1 = do forecast for the next period
		//       or F2 = do forecast for the period after the next period
		rlsClass* ELS;
		void F_els();
	// case sg	140 val yPointer y_num  varphi_num  varphiPointer_i vpP_length_i vpP_lag_i  mum  Forecast
		//	140 1 q 1  3  p 1 2  q 1 2 d 1 1  2  F1
		//          val = initvalue of approach factor a 
		// 	    q 1 = forecast portfolio q with one asset
		//	    p 1 3 = input is old 3 prices of one asset
		//          num 2 = lag of estimate error correction
		//          F1 = do forecast for the next period
		//       or F2 = do forecast for the period after the next period
		//       or Ux sigma epsilon startmatrix = do unbiased forecast for the period x
		//       or Ux sigma epsilon varname startmatrix = do unbiased forecast for the period x
		// remark: in Ux case the second inputvector will be used for initializing and replaced internal with ye!!!
		// remark: in Ux case the third inputvector can be replaced with varname in function unbasiasedForecast
		sgClass* SG;
		real firstperiod,sigma,epsilon;
		real* ipointer; // input pointer
		real* tstart; // theta start matrix
		void F_sg();

	// case statistics 200 inputvarname
		char  statvarname[40];
		real* statvar;
		real  mean,variance,standardDeviation,meansum,variationskoeff,sharpeRatio;

	// case quantiles 210 inputvarname
		char quanvarname[40];
		double x, zx, sk, ku, xi, mr[4], mc[4], *quanvar, sd;
		real u;
		void Sequper(const long&);
		void Bisec();
		void FXRoot();
		double FX();
		double FX1();
		double FX2();
		double PHIX();
		void ZX();
		
public:
		manipulate();
		virtual ~manipulate();

		real* setLabels(char*);
		long getLength() const;
		real* sendModelVar();
		void sendStateSpace(int &,const real***);
		void receiveParameters(const real*);
		void sendParameters(int&,real**);
		void printParamset();
		void saveParamset(ofstream&);

		void saveParamsetWithNames(ofstream&);
		void loadParamset(ifstream&);
		void initialize();
		void iteration(const long&);
};

#endif
