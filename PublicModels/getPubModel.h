///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		getPubModel.h
// Contents:		two functions converting strings to models
//
// Author:		Marc Mueller 98/07/26
// Last modified:	98/08/21 Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _GETPUBMODEL_INCLUDED
#define _GETPUBMODEL_INCLUDED

#include "baseModel.h"

void PrintPubModelDate(void);

///////////////////////////////////////////////////////////////////////////////
//
//
// Function:		getPubModel(const char*)
// Purpose:		returns a new model for type modelTyp
//
// Author:		Marc Mueller 98/07/26
// Last modified:	Sun Jul 26 13:29:21 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
baseModel* getPubModel(const QString& modelTyp);

///////////////////////////////////////////////////////////////////////////////
//
//
// Function:		getModel(const char*, const baseModel*)
// Purpose:		returns a new model for type modelTyp from sourceModel
//
// Author:		Marc Mueller 98/07/26
// Last modified:	Sun Jul 26 13:29:21 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
baseModel* getPubModel(const QString& modelTyp, const baseModel* sourceModel);

#endif // _GETPUBMODEL_INCLUDED

// eof
