///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/getModel.h,v 1.1 2000/08/31 15:56:03 mmeyer Exp $
//
// Module name:		getmodel.h
// Contents:		two functions converting strings to models
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 17:20:25 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _GETMODEL_INCLUDED
#define _GETMODEL_INCLUDED

#include "baseModel.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Function:		getModel(const char*)
// Purpose:		returns a new model for type modelTyp
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
baseModel* getModel(const char* modelTyp);

///////////////////////////////////////////////////////////////////////////////
//
//
// Function:		getModel(const char*, const baseModel*)
// Purpose:		returns a new model for type modelTyp from sourceModel
//
// Author:		Uli Middelberg
// Last modified:	Mon Mar 10 14:44:08 MET 1997
// By:			Uli Middelberg
//
///////////////////////////////////////////////////////////////////////////////
baseModel* getModel(const char* modelTyp, const baseModel* sourceModel);

#endif // _GETMODEL_INCLUDED

// eof

