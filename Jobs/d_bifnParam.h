// File:	d_bifnParam.h
// Contents:	definition of class nParameterBifurcation with density
// Author:	Andreas Starke
// Creation:	
// Modified:	
// Changes:	

#ifndef D_BIFNPARAM_H
#define D_BIFNPARAM_H

#include "bifnParam.h"
#include "histo.h"
/******************************************************************************/
/*                                                                            */
/* Class name:    d_Bif_n_Parameter                                           */
/* Derived from:  bifnParam	                                              */
/* Purpose:       compute a density bifurcation for an                        */
/*                x-bundle for n parameters  				      */
/*                                                                            */
/* Author:        Andreas Starke                                              */
/* Last modified:                                                             */
/*                                                                            */
/******************************************************************************/

class d_bifnParam : public bifnParam {

protected:
	ofstream outFile;
	histo_1d h;		// hit storing grid
    qreal stepY;		// scale of the grid
	int resolution_x;	// store dummy
	
public:
	d_bifnParam(baseModel* const, 
		const xyRange&,const xyRange&,
        MacrodynGraphicsItem* const=NULL);
	virtual ~d_bifnParam();
	void simulation();
};    

#endif
//eof
