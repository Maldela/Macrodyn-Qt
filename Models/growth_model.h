///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/growth_model.h,v 1.3 2002/01/08 09:04:26 mmeyer Exp mmeyer $
//
// Module name:		growth_model.h
// Contents:		Class definition of the class growth_model,
//			a dynamik growth model including caldor
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 12:40:17 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _GROWTH_MODEL_INCLUDED
#define _GROWTH_MODEL_INCLUDED

#include "baseModel.h"
#include "rand_var.h"
//#include <compat.h>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		growth_model
// Derived from:	baseModel
// Purpose:		defines the basic growth_model
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 12:40:08 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

class growth_model : public baseModel {

protected:
    			// original state variables
    qreal K;		// kapital
    qreal L;		// labour
    qreal ln_K;		// ln of K
    qreal ln_L;		// ln of L
    qreal d_K_kL;	// difference K_t - k_fix L_t
    qreal k;		// transformed state variable k:=K/L
    qreal k_fix;		// fixed point of k

    qreal k_0;		// initial value
    
    			// derived state variables
    qreal y;		// output
    qreal w;		// wage
    qreal r;		// rent
    qreal xBundle;

				// production function
    qreal ( * pf ) ( qreal k, qreal a, qreal b, qreal c, qreal alpha );
    				// a pointer to the production function
    qreal ( * pf_prime) ( qreal k, qreal a, qreal b, qreal c, qreal alpha );
    				// and its derivative
    qreal ( * pf_second_prime) ( qreal k, qreal a, qreal b, qreal c, qreal alpha );
    				// and its derivative
    qreal pf_type;		// the current type of the production function
    qreal a, b, c, alpha;	// original parameters
    qreal A, B, C, D;		// parameters as ][0,1][ normalized input
    void pf_init ( void );	// maps (A,B,C) -> (a,b,c) and sets pf, pf_prime
    
    virtual qreal sf_y ( qreal );	// savings function for the entire economy
    qreal syr;			// parameter for sf_y
    virtual qreal sf_w ( qreal ); // savings function for the worker w.r.t the wage
    qreal swr;			// parameter for sf_w
    virtual qreal sf_r ( qreal ); // savings function for the shareholder w.r.t the rent
    qreal srr;			// parameter for sf_r
    
     			// derived state variables
    qreal sav_y;		// savings w.r.t. the output
    qreal sav_w;		// savings w.r.t. the wage
    qreal sav_r;		// savings w.r.t. the rent

    qreal n;		// exogenous growth of labour suppling people
   
    qreal delta_p;	// depreciation of kapital
    qreal s_hat;
    qreal Ef_sc;

public:
    growth_model();			// constructor
    void loadParamset(QTextStream&);	
    void saveParamset(QTextStream&);
    void printParamset();
    void iteration(const qint64&);
    void initialize();
    qreal* setLabels(const QString&);
    void sendStateSpace(int &,const qreal***);

    qreal* sendModelVar();		// for compatibity reasons only
    void sendParameters(int&,qreal**);
    void receiveParameters(const QList<qreal>&);

};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          rgrowth_model
// Derived from:        growth_model
// Purpose:             defines the growth_model with random variables
//
// Author:              Uli Middelberg
// Last modified:       Tue Aug 19 14:26:59 METDST 1997
// By:                  Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

class rgrowth_model : public growth_model {
private:
    QString zvar_expr;        // expression which defines a random variable
    rand_var * zvar;            // a random variable    
    QString zvar_name;        // name of the stochastified parameter
    qreal * zvar_ptr;		// pointer to the stochastic parameter
    
public:
    rgrowth_model();			// constructor
    void loadParamset(QTextStream&);	// load parameterset from a file
    void iteration(const qint64&);	// perform one iteration 
    void initialize(void);		// initialize the model
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          depreciate
// Derived from:        growth_model
// Purpose:             defines the growth_model with several kinds
//			of chaotic depreciation rates
//			delta_p becomes a state variable now
//
// Author:              Uli Middelberg
// Last modified:       Fri Jun 12 14:48:41 METDST 1998
// By:                  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
const int trans_x_max=100; 		// Max. number of i in trans_x[i]

class depreciate : public growth_model {

private:
    qreal _z;				// variable for the original process
    qreal _z_0;				// initial value for z
    QString _z_var_name;		// lexical name of the stochastified parameter
    qreal * _z_var_ptr;			// pointer to the stochastified parameter

    qreal trans(qreal z,qreal* x,qreal* a,qreal* b); // transforms z to delta_p
    qreal * trans_x;			// pointer to array classes
    qreal * trans_a;			// parameter for the transformation
    qreal * trans_b;			// parameter for the transformation

    qreal (* theta) (qreal z, qreal m);	// theta defines the basic dynamic for z
    qreal theta_type;			// the current type of the theta function
    qreal my;				// parameters for theta
    void theta_init (void);		// sets theta to a specific type
    
public:
    depreciate();			// constructor
    void sendStateSpace(int &,const qreal***);
    qreal * setLabels(const QString&);		// get a variable by a name
    void initialize(void);		// initialize the model
    void loadParamset(QTextStream&);	// load parameterset from a file
    void saveParamset(QTextStream&);
    void printParamset();
    void iteration(const qint64&);	// perform one iteration 
};

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:          pasinetti
// Derived from:        growth_model
// Purpose:             defines the growth_model with several kinds
//			of chaotic depreciation rates
//			delta_p becomes a state variable now
//
// Author:              Marc Mueller
// Last modified:       Mon Dec  7 18:54:51 MET 1998
// By:                  Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

class pasinetti : public growth_model {
private:
    //// k_0                               initial value shareholder
    qreal k_1;                           // initial value worker
    qreal kr,kw,lohn,kap_an,kap_ka,eink_an,kap_ges;		//Kapital KA, Kapital AN, Lohn AN
    qreal lohn_od,kap_an_od,kap_ka_od;
    qreal Q_r;
    qreal Q_w;
    qreal Q_r_n;
    qreal Q_w_n;
    qreal Quot_kr;
    qreal Quot_kw;
    qreal k_pas;
    qreal E;
    qreal E_n;
    qreal E_f_pr_n, bif_point;
    qreal E_steady_leon;
    qreal E_steady_quad;
    qreal E_steady_leon_prime;
    qreal E_steady_quad_prime;
    qreal y_ini,y_ini_prime;
    qreal y_quad,y_quad_prime,y_quad_prime2;
    qreal y_leon,y_leon_prime,y_leon_prime2;
    qreal xBundle;
    qreal YBundle;
    qreal steady_quad;
    qreal steady_leon;
    qreal steady_kc_leon;
    qreal steady_kw_leon;
    qreal steady_kc_quad;
    qreal steady_kw_quad;
    qreal steady_kc;
    qreal steady_kw;
    qreal steady_cd;
    qreal steady_kc_cd;
    qreal steady_kw_cd;
    qreal steady_ces;
    qreal y_steady;
    qreal y_prime_steady;
    qreal E_f;
	

    qreal Q;
    qreal Konsum_C, Konsum_K, Konsum_A, Konsum_C_n, Konsum_A_n, Z, Z2;

public:
    pasinetti();			// constructor
	virtual ~pasinetti();
    void loadParamset(QTextStream&);	// load parameterset from a file
    void saveParamset(QTextStream&);
    void printParamset();
    qreal * setLabels(const QString&);		// get a variable by a name
    void initialize(void);		// initialize the model
    void iteration(const qint64&);	// perform one iteration 
};



#endif //  _GROWTH_MODEL_INCLUDED

// eof
