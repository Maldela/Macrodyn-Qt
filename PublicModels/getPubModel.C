///////////////////////////////////////////////////////////////////////////////
//
//
// Module name:		getPubModel.C
// Contents:		two functions converting strings to models
//
// Author:		Markus Lohmann Marc Mueller 98/07/26
// Last modified:	Fri Jul 31 15:03:52 METDST 1998
// By:			Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#include "defaultModel.h"
#include "../Models/logistic.h"
#include "../Models/henon.h"
#include "perturbedDelayedLogisticMap.h"
#include "cobweb.h"

#include "../Models/affine1.h"
#include "../Models/affine2.h"
#include "../Models/affine3.h"
#include "../Models/HicksModel.h"
#include "../Models/Keener.h"
#include "../Models/logiDelay.h"
#include "../Models/onozaki_et_al.h"
#include "../Models/rSolow.h"

#include "../error.h"

///////////////////////////////////////////////////////////////////////////////
//
// Function:		PrintPubModelDate
// Purpose:		print date an time on std output
// Author:		Marc Mueller 98/08/21
// Last modified:	98/08/21
//
///////////////////////////////////////////////////////////////////////////////
void PrintPubModelDate(void) {
      #ifdef __DATE__
        #ifdef __TIME__
          log() << "public macrodyn  built on " << __DATE__ << " (" << __TIME__ << ")\n";
        #else
          log() << "public macrodyn  built on " << __DATE__ << "\n";
        #endif
      #else
        log() << "Sorry, no build date stored of public macrodyn.\n";
      #endif
};

///////////////////////////////////////////////////////////////////////////////
//
// Function:		getPubModel(const char*)
// Purpose:		returns a new model for type modelTyp
// Author:		Marc Mueller 98/07/26
// Last modified:	Fri Jul 31 15:04:00 METDST 1998 Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
baseModel* getPubModel(const char* modelTyp)
{
    if(!strcmp(modelTyp,"default") )
	return new defaultModel;
    if(!strcmp(modelTyp,"rdefault") )
	return new rdefaultModel;
    if( !strcmp(modelTyp,"henon") )
	return new henon;
    if( !strcmp(modelTyp,"quadHenon") )
	return new quadHenon;
    if( !strcmp(modelTyp,"logistic") )
	return new logistic;
    if( !strcmp(modelTyp,"rlogistic") )
	return new rlogistic;
    if( !strcmp(modelTyp,"perturbedDelayedLogisticMap") )
	return new perturbedDelayedLogisticMap;
    if(!strcmp(modelTyp,"cobweb") )
	return new cobweb;
    if(!strcmp(modelTyp,"cobweb_RLS") )
	return new cobweb_RLS;
    if(!strcmp(modelTyp,"rdemand_cobweb") )
	return new rdemand_cobweb;

    if(!strcmp(modelTyp,"affine1") )
	return new affine1;

    if(!strcmp(modelTyp,"affine2") )
	return new affine2; 

    if(!strcmp(modelTyp,"affine3") )
	return new affine3;

    if(!strcmp(modelTyp,"Keener") )
	return new Keener;

    if(!strcmp(modelTyp,"HicksModel") )
	return new HicksModel;

    if(!strcmp(modelTyp,"logiDelay") )
	return new logiDelay;

    if(!strcmp(modelTyp,"onozaki_et_al") )
	return new onozaki_et_al;

    if(!strcmp(modelTyp,"rSolow") )
	return new rSolow;

    error("macrodyn::getPubModel(const char*): Model %s not implemented yet", modelTyp);
    return NULL;
} 

///////////////////////////////////////////////////////////////////////////////
//
// Function:		getModel(const char*, const baseModel*)
// Purpose:		returns a new model for type modelTyp from sourceModel
// Author:		Marc Mueller 98/07/26
// Last modified:	Wed May 19 11:30:13 METDST 1999 Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////
baseModel* getPubModel(const char* modelTyp, const baseModel* sourceModel)
{
    if(!strcmp(modelTyp,"default") )
	return new defaultModel(*((defaultModel*)sourceModel));
    if(!strcmp(modelTyp,"rdefault") )
	return new rdefaultModel(*((rdefaultModel*)sourceModel));
    if(!strcmp(modelTyp,"henon") )
	return new henon(*((henon*)sourceModel));
    if(!strcmp(modelTyp,"quadHenon") )
	return new quadHenon(*((quadHenon*)sourceModel));
    if(!strcmp(modelTyp,"logistic") )
	return new logistic(*((logistic*)sourceModel));
    if(!strcmp(modelTyp,"rlogistic") )
	return new rlogistic(*((rlogistic*)sourceModel));
    if( !strcmp(modelTyp,"perturbedDelayedLogisticMap") )
	return new perturbedDelayedLogisticMap(*((perturbedDelayedLogisticMap*)sourceModel));
    if(!strcmp(modelTyp,"cobweb") )
	return new cobweb(*((cobweb*)sourceModel));
    if(!strcmp(modelTyp,"cobweb_RLS") )
	return new cobweb_RLS(*((cobweb_RLS*)sourceModel));
    if(!strcmp(modelTyp,"rdemand_cobweb") )
	return new rdemand_cobweb(*((rdemand_cobweb*)sourceModel));

    if( !strcmp(modelTyp,"affine1") )
	return new affine1(*((affine1*)sourceModel));	
    if( !strcmp(modelTyp,"affine2") )
	return new affine2(*((affine2*)sourceModel));
    if( !strcmp(modelTyp,"affine3") )
	return new affine3(*((affine3*)sourceModel));
    if( !strcmp(modelTyp,"logiDelay") )
	return new logiDelay(*((logiDelay*)sourceModel));
    if( !strcmp(modelTyp,"HicksModel") )
	return new HicksModel(*((HicksModel*)sourceModel));
    if( !strcmp(modelTyp,"Keener") )
	return new Keener(*((Keener*)sourceModel));
    if( !strcmp(modelTyp,"onozaki_et_al") )
	return new onozaki_et_al(*((onozaki_et_al*)sourceModel));
    if( !strcmp(modelTyp,"rSolow") )
	return new rSolow(*((rSolow*)sourceModel));

    error("macrodyn::getPubModel(const char*, const baseModel*): Model %s not implemented yet", modelTyp);
    return NULL;		// not reached
}
