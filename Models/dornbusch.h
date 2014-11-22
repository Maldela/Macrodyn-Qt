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
   
   qreal M;
   qreal Yr;
   qreal pA;
   qreal iA;
   qreal it;
   qreal pt;
   qreal ptr;
   qreal pt_0;
   qreal xet_0;
   qreal xet;
   qreal xetr;
   qreal pi;
   qreal u;
   qreal delta;
   qreal gamma;
   qreal sigma;
   qreal lambda;
   qreal theta;

public:
    dornbusch();			// constructor
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void printParamset();
    void iteration(const long&);
    void initialize();
    qreal* setLabels(char*);
    void sendStateSpace(int &,const qreal***);

    qreal* sendModelVar();		// for compatibity reasons only
    void sendParameters(int&,qreal**);
    void receiveParameters(const qreal*);
    virtual ~dornbusch(void);
};

#endif //  _DORNBUSCH_INCLUDED

// eof
