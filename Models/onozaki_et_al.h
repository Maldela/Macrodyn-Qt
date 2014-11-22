///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/onozaki_et_al.h,v 1.1 2000/08/31 16:07:50 mmeyer Exp $
//
// Module name:		onozaki_et_al.h
// Contents:		Class definition of the class onozaki_et_al,
//			a dynamik growth model including caldor
//
// Author:		Uli Middelberg
// Last modified:	Mon May 11 12:40:17 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _onozaki_et_al_INCLUDED
#define _onozaki_et_al_INCLUDED

#include "baseModel.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Class name:		onozaki_et_al
// Derived from:	baseModel
// Purpose:		defines the basic onozaki_et_al
//
// Author:		Mathias Hoffmann
// Last modified:	Mon March 11 12:40:08 METDST 2000
// By:			Mathias Hoffmann
//
///////////////////////////////////////////////////////////////////////////////

class onozaki_et_al : public baseModel {

protected:
    			
    qreal z_0;		// initial value
    qreal z;			// argument
    qreal alpha, beta;	// original parameters
    
public:
    onozaki_et_al();			// constructor
    void loadParamset(ifstream&);	
    void saveParamset(ofstream&);
    void saveParamsetWithNames(ofstream&);
    void printParamset();
    void iteration(const long&);
    void initialize();
    qreal* setLabels(char*);
    void sendStateSpace(int &,const qreal***);

    qreal* sendModelVar();		// for compatibity reasons only
    void sendParameters(int&,qreal**);
    void receiveParameters(const qreal*);

};

#endif //  _onozaki_et_al_INCLUDED

// eof