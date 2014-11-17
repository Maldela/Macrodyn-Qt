///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/dornbusch.h,v 1.1 2000/08/31 15:48:38 mmeyer Exp $
//
// Module name:		dornbusch.h
// Contents:		Class definition of the class growth_model_m,
//			a dynamik growth model 
//
// Author:		Michael Meyer
// Last modified:	Mon Mar 15 12:40:17 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _DORNBUSCH_INCLUDED
#define _DORNBUSCH_INCLUDED

#include "baseModel.h"


///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		dornbusch
// Derived from:	baseModel
// Purpose:		defines the basic growth_model
//
// Author:		Michael Meyer
// Last modified:	Mon Mar 15 12:40:08 METDST 1999
// By:			Michael Meyer
//
///////////////////////////////////////////////////////////////////////////////

class dornbusch : public baseModel {

protected:
   
   real M;
   real Yr;
   real pA;
   real iA;
   real it;
   real pt;
   real ptr;
   real pt_0;
   real xet_0;
   real xet;
   real xetr;
   real pi;
   real u;
   real delta;
   real gamma;
   real sigma;
   real lambda;
   real theta;

public:
    dornbusch();			// constructor
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void printParamset();
    void iteration(const long&);
    void initialize();
    real* setLabels(char*);
    void sendStateSpace(int &,const real***);

    real* sendModelVar();		// for compatibity reasons only
    void sendParameters(int&,real**);
    void receiveParameters(const real*);
    virtual ~dornbusch(void);
};

#endif //  _DORNBUSCH_INCLUDED

// eof
