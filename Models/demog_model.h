///////////////////////////////////////////////////////////////////////////////
//
// Module name:		demog_model.h
// Contents:		Class definition of the class demog_model
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _DEMOGMODEL_INCLUDED
#define _DEMOGMODEL_INCLUDED

#include "baseModel.h"
#include "../matrix_neu.h"

///////////////////////////////////////////////////////////////////////////////
//
// Class name:		demog_model
// Derived from:	baseModel
// Purpose:		
//
// Author:		Andreas Starke
// Last modified:	
//
///////////////////////////////////////////////////////////////////////////////

class demog_model : public baseModel {

protected:
	qreal w_1,w_2,w_3,R_1,R_2;
	qreal w_0, R_0;
	qreal K_0,K_t,n0_t,n1_t,n2_t,n3_t,n0_0,n1_0,n2_0,n3_0;
	qreal eta0, eta1, eta2, eta0_old, eta1_old, eta2_old;
	
	qreal k_t,w_t,L_t,R_t,r_t,S_t,s1_0_t, s1_1_t, s1_2_t;
	qreal W_0,W_1,W_2;
	qreal s2_0_t, s2_1_t, s2_2_t;
	qreal alpha0,alpha1,alpha2;
	qreal alpha_max, c_child, varphi, l_1,l_2;
	qreal tax_rate_0, tax_rate_1, tax_rate_2;
	
	qreal A,B,delta;
	int type;


	qreal R_old, R_1_old, R_2_old;
	qreal s1_0_old, s1_1_old, s1_2_old;
	
	qreal theta_L, theta_K, theta_N, theta_W, theta_k;
	
public:
    demog_model();			// constructor
    virtual ~demog_model();			// destructor
    void loadParamset(QTextStream&);
    void saveParamset(QTextStream&);
    void saveParamsetWithNames(QTextStream&);
    void printParamset();
    void iteration(const qint64&);
    void initialize();
    qreal* sendModelVar();
    qreal* setLabels(const QString&);
    void sendStateSpace(int &, QList<qreal *> *);
    void sendParameters(int&,qreal**); 	// write all parameters
                                	// into an array and return the 
                                	// numbers of parameters
                                	
    void receiveParameters(const QList<qreal>&);// receive parameter values 
};

#endif	
// eof
