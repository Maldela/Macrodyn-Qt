///////////////////////////////////////////////////////////////////////////////
//
// $Header: /home/vwlt1/mmeyer/macrodyn_dmdyn/src/Models/RCS/getModel.C,v 1.9 2002/01/08 09:30:08 mmeyer Exp mmeyer $
//
// Module name:		getmodel.C
// Contents:		two functions converting strings to models
//
// Author:		Uli Middelberg
// Last modified:	Sun Jun 20 20:13:34 CEST 1999 Marc Mueller
//
///////////////////////////////////////////////////////////////////////////////

#include "newMacro.h"
#include "geoExp.h"
#include "adaptExp.h"
#include "affine1.h"
#include "affine2.h"
#include "affine3.h"
#include "constantLag.h"
#include "radio.h"
#include "logistic.h"
#include "henon.h"
#include "logiDelay.h"
#include "linDiff.h"
#include "martin.h"
#include "capOlg.h"
#include "excOlg.h"
#include "laborMarketFirst.h"
#include "leo.h"
#include "laborMarketFirstTax.h"
#include "OlgRat.h"
#include "defbudba.h"
#include "Monopol.h"
#include "Duopol.h"
#include "cobweb_g_predictor.h"
#include "toniRV.h"
#include "tonicapm.h"
#include "tonicapmLag.h"
#include "toniLag.h"
#include "toni2.h"
#include "toniadp.h"
#include "toniadp2.h"
#include "hetoni.h"
#include "growth_model.h"
#include "nicole.h"
#include "jan_model.h"
#include "rSolow.h"
#include "dynofin_rmv.h"
#include "armax.h"
#include "armax_eindim.h"
#include "rOLG_wt.h"
#include "dornbusch.h"
#include "dorn.h"
#include "samuelson1.h"
#include "frepm.h"
#include "manipulate.h"
#include "linFima_basic.h"
#include "linFimaErwRueck.h"
#include "linFima_wage.h"
#include "pureExchange.h"
#include "martinNeu.h"
#include "onozaki_et_al.h"
#include "MerA.h"
#include "Bankintermed.h"
#include "finanzmarkt_2Dim_1.h"
#include "finanzmarkt_wage.h"
#include "finanzmarkt_JanE.h"
#include "ExchangeRateModel.h"
#include "ExchangeRateModel2.h"
#include "ELS.h"
#include "demog_model.h"
#include "HicksModel.h"
#include "RBC_basic_model.h"
#include "Keener.h"
#include "Samuelson_basic.h"

#include "test_model.h"

#include "../error.h"

///////////////////////////////////////////////////////////////////////////////
//
//
// Function:		getModel(const char*)
// Purpose:		returns a new model for type modelTyp
//
// Author:		Markus Lohmann
//
///////////////////////////////////////////////////////////////////////////////
baseModel* getModel(const char* modelTyp)
{
    if( !strcmp(modelTyp,"radio") )
	return new radio;
    if( !strcmp(modelTyp,"newMacro") )
	return new newMacro;
    if( !strcmp(modelTyp,"geoExp") )
	return new geoExp;
    if( !strcmp(modelTyp,"adaptExp") )
	return new adaptExp;
    if( !strcmp(modelTyp,"affine1") )
	return new affine1;	
    if( !strcmp(modelTyp,"affine2") )
	return new affine2;
    if( !strcmp(modelTyp,"affine3") )
	return new affine3;
    if( !strcmp(modelTyp,"constantLag") )
	return new constantLag;
    if( !strcmp(modelTyp,"logistic") )
	return new logistic;
    if( !strcmp(modelTyp,"rlogistic") )
	return new rlogistic;
    if( !strcmp(modelTyp,"henon") )
	return new henon;
    if( !strcmp(modelTyp,"quadHenon") )
	return new quadHenon;
    if( !strcmp(modelTyp,"logiDelay") )
	return new logiDelay;
    if( !strcmp(modelTyp,"logLogDelay") )
	return new logLogDelay;
    if( !strcmp(modelTyp,"linDiff") )
	return new linDiff;
    if( !strcmp(modelTyp,"martin") )
	return new martin;
    if( !strcmp(modelTyp,"martin2") )
	return new martin2;
    if( !strcmp(modelTyp,"martin3") )
	return new martin3;
    if( !strcmp(modelTyp,"capOlg") )
	return new capOlg;
    if( !strcmp(modelTyp,"capOlgAdapt") )
	return new capOlgAdapt;
    if( !strcmp(modelTyp,"capOlgGeoExp") )
	return new capOlgGeoExp;
    if( !strcmp(modelTyp,"excOlg") )
	return new excOlg;
    if( !strcmp(modelTyp,"excOlgAdapt") )
	return new excOlgAdapt;
    if( !strcmp(modelTyp,"excOlgGeoExp") )
	return new excOlgGeoExp;
    if( !strcmp(modelTyp,"laborMarketFirst") )
	return new laborMarketFirst;
    if( !strcmp(modelTyp,"leo") )
	return new leo;
    if( !strcmp(modelTyp,"laborMarketFirstTax") )
	return new laborMarketFirstTax;
    if( !strcmp(modelTyp,"rlaborMarketFirstTax") )
    	return new rlaborMarketFirstTax;
    if( !strcmp(modelTyp,"martin4") )
	return new martin4;
    if( !strcmp(modelTyp,"OlgRat") )
	return new OlgRat;
    if( !strcmp(modelTyp,"OlgRat2") )
	return new OlgRat2;
    if( !strcmp(modelTyp,"defbudba") )
	return new defbudba;
    if( !strcmp(modelTyp,"Monopol") )
	return new Monopol;
    if( !strcmp(modelTyp,"Duopol") )
	return new Duopol;
    if( !strcmp(modelTyp,"Duopol2") )
	return new Duopol2;
    if( !strcmp(modelTyp,"cobweb_g_predictor") )
    return new cobweb_g_predictor;
    if( !strcmp(modelTyp,"toniRV") )
    return new toniRV;
    if( !strcmp(modelTyp,"tonicapm") )
    return new tonicapm;
    if( !strcmp(modelTyp,"tonicapmLag") )
    return new tonicapmLag;
    if( !strcmp(modelTyp,"toniLag") )
    return new toniLag;
    if( !strcmp(modelTyp,"toniadp") )
    return new toniadp;
    if( !strcmp(modelTyp,"toniadp2") )
    return new toniadp;
    if( !strcmp(modelTyp,"toni2") )
    return new toni2;
    if( !strcmp(modelTyp,"hetoni") )
	return new hetoni;
    if( !strcmp(modelTyp,"growth_model") )
	return new growth_model;
    if( !strcmp(modelTyp,"rgrowth_model") )
	return new rgrowth_model;
    if( !strcmp(modelTyp,"depreciate") )
	return new depreciate;
    if( !strcmp(modelTyp,"pasinetti") )
	return new pasinetti;
    if( !strcmp(modelTyp,"nicole") )
	return new nicole;
    if( !strcmp(modelTyp,"jan_model") )
	return new jan_model;
    if( !strcmp(modelTyp,"rSolow") )
	return new rSolow;
    if( !strcmp(modelTyp,"RBC_delta_1") )
	return new RBC_delta_1;    
    if( !strcmp(modelTyp,"dynofin_rmv") )
	return new dynofin_rmv;
    if( !strcmp(modelTyp,"armax_eindim") )
	return new armax_eindim;
    if( !strcmp(modelTyp,"armax") )
	return new armax;
    if( !strcmp(modelTyp,"rOLG_wt") )
	return new rOLG_wt;
    if( !strcmp(modelTyp,"dornbusch") )
	return new dornbusch;
    if( !strcmp(modelTyp,"dorn") )
	return new dorn;
    if( !strcmp(modelTyp,"samuelson1") )
	return new samuelson1;
    if( !strcmp(modelTyp,"frepm") )
	return new frepm;
    if( !strcmp(modelTyp,"manipulate") )
	return new manipulate;
    if( !strcmp(modelTyp,"linFima_basic") )
	return new linFima_basic;
    if( !strcmp(modelTyp,"linFimaErwRueck") )
	return new linFimaErwRueck;
    if( !strcmp(modelTyp,"linFima_wage") )
	return new linFima_wage;
    if( !strcmp(modelTyp,"pureExchange") )
	return new pureExchange;
    if( !strcmp(modelTyp,"martinNeu") )
	return new martinNeu;
    if( !strcmp(modelTyp,"onozaki_et_al") )
	return new onozaki_et_al;
    if( !strcmp(modelTyp,"MerA") )
	return new MerA;
    if( !strcmp(modelTyp,"Bankintermed") )
	return new Bankintermed;
	if( !strcmp(modelTyp,"finanzmarkt_2Dim_1") )
	return new finanzmarkt_2Dim_1;
	if( !strcmp(modelTyp,"finanzmarkt_wage") )
	return new finanzmarkt_wage;
	if( !strcmp(modelTyp,"finanzmarkt_JanE") )
	return new finanzmarkt_JanE;
	if( !strcmp(modelTyp,"ExchangeRateModel") )
	return new ExchangeRateModel;
	if( !strcmp(modelTyp,"ExchangeRateModel2") )
	return new ExchangeRateModel2;	
	if( !strcmp(modelTyp,"ELS") )
	return new ELS;
	if( !strcmp(modelTyp,"demog_model") )
	return new demog_model;	
	if( !strcmp(modelTyp,"HicksModel") )
	return new HicksModel;
	if( !strcmp(modelTyp,"RBC_basic_model") )
	return new RBC_basic_model;
	if( !strcmp(modelTyp,"Keener") )
	return new Keener;
	if( !strcmp(modelTyp,"Samuelson_basic") )
	return new Samuelson_basic;
	if( !strcmp(modelTyp,"Samuelson_ar1") )
	return new Samuelson_ar1;
	
	if( !strcmp(modelTyp,"test_model") )
	return new test_model;			
    return NULL;
}//getModel(1)


///////////////////////////////////////////////////////////////////////////////
//
//
// Function:		getModel(const char*, const baseModel*)
// Purpose:		returns a new model for type modelTyp from sourceModel
//
// Author:		Markus Lohmann
//
///////////////////////////////////////////////////////////////////////////////
baseModel* getModel(const char* modelTyp,const baseModel* sourceModel)
{
    if( !strcmp(modelTyp,"radio") )
	return new radio(*((radio*)sourceModel));
    if( !strcmp(modelTyp,"newMacro") )
	return new newMacro(*((newMacro*)sourceModel));
    if( !strcmp(modelTyp,"logistic") )
	return new logistic(*((logistic*)sourceModel));
    if( !strcmp(modelTyp,"rlogistic") )
	return new rlogistic(*((rlogistic*)sourceModel));
    if( !strcmp(modelTyp,"geoExp") )
	return new geoExp(*((geoExp*)sourceModel));
    if( !strcmp(modelTyp,"adaptExp") )
	return new adaptExp(*((adaptExp*)sourceModel));
    if( !strcmp(modelTyp,"affine1") )
	return new affine1(*((affine1*)sourceModel));	
    if( !strcmp(modelTyp,"affine2") )
	return new affine2(*((affine2*)sourceModel));
    if( !strcmp(modelTyp,"affine3") )
	return new affine3(*((affine3*)sourceModel));
    if( !strcmp(modelTyp,"constantLag") )
	return new constantLag(*((constantLag*)sourceModel));
    if( !strcmp(modelTyp,"henon") )
	return new henon(*((henon*)sourceModel));
    if( !strcmp(modelTyp,"quadHenon") )
	return new quadHenon(*((quadHenon*)sourceModel));
    if( !strcmp(modelTyp,"logiDelay") )
	return new logiDelay(*((logiDelay*)sourceModel));
    if( !strcmp(modelTyp,"logLogDelay") )
	return new logLogDelay(*((logLogDelay*)sourceModel));
    if( !strcmp(modelTyp,"linDiff") )
	return new linDiff(*((linDiff*)sourceModel));
    if( !strcmp(modelTyp,"martin") )
	return new martin(*((martin*)sourceModel));
    if( !strcmp(modelTyp,"martin2") )
	return new martin2(*((martin2*)sourceModel));
    if( !strcmp(modelTyp,"martin3") )
	return new martin3(*((martin3*)sourceModel));
    if( !strcmp(modelTyp,"capOlg") )
	return new capOlg(*((capOlg*)sourceModel));
    if( !strcmp(modelTyp,"capOlgAdapt") )
	return new capOlgAdapt(*((capOlgAdapt*)sourceModel));
    if( !strcmp(modelTyp,"capOlgGeoExp") )
	return new capOlgGeoExp(*((capOlgGeoExp*)sourceModel));
    if( !strcmp(modelTyp,"excOlg") )
	return new excOlg(*((excOlg*)sourceModel));
    if( !strcmp(modelTyp,"excOlgAdapt") )
	return new excOlgAdapt(*((excOlgAdapt*)sourceModel));
    if( !strcmp(modelTyp,"excOlgGeoExp") )
	return new excOlgGeoExp(*((excOlgGeoExp*)sourceModel));
    if( !strcmp(modelTyp,"laborMarketFirst") )
	return new laborMarketFirst(*((laborMarketFirst*)sourceModel));
    if( !strcmp(modelTyp,"leo") )
	return new leo(*((leo*)sourceModel));
    if( !strcmp(modelTyp,"laborMarketFirstTax") )
	return new laborMarketFirstTax(*((laborMarketFirstTax*)sourceModel));
    if( !strcmp(modelTyp,"rlaborMarketFirstTax") )
        return new rlaborMarketFirstTax(*((rlaborMarketFirstTax*)sourceModel));
    if( !strcmp(modelTyp,"martin4") )
	return new martin4(*((martin4*)sourceModel));
    if( !strcmp(modelTyp,"OlgRat") )
	return new OlgRat(*((OlgRat*)sourceModel));
    if( !strcmp(modelTyp,"OlgRat2") )
	return new OlgRat2(*((OlgRat2*)sourceModel));
    if(!strcmp(modelTyp,"defbudba") )
	return new defbudba(*((defbudba*)sourceModel));
    if(!strcmp(modelTyp,"Monopol") )
	return new Monopol(*((Monopol*)sourceModel));
    if(!strcmp(modelTyp,"Duopol") )
	return new Duopol(*((Duopol*)sourceModel));
    if( !strcmp(modelTyp,"Duopol2") )
	return new Duopol2(*((Duopol2*)sourceModel));
    if( !strcmp(modelTyp,"cobweb_g_predictor") )
        return new cobweb_g_predictor(*((cobweb_g_predictor*)sourceModel));
    if( !strcmp(modelTyp,"toniRV") )
        return new toniRV(*((toniRV*)sourceModel));
    if( !strcmp(modelTyp,"tonicapm") )
      return new tonicapm(*((tonicapm*)sourceModel));
    if( !strcmp(modelTyp,"tonicapmLag") )
      return new tonicapmLag(*((tonicapmLag*)sourceModel));
    if( !strcmp(modelTyp,"toniLag") )
	return new toniLag(*((toniLag*)sourceModel));
    if( !strcmp(modelTyp,"toniadp") )
	return new toniadp(*((toniadp*)sourceModel));
    if( !strcmp(modelTyp,"toniadp2") )
	return new toniadp2(*((toniadp2*)sourceModel));
    if( !strcmp(modelTyp,"toni2") )
	return new toni2(*((toni2*)sourceModel));
    if( !strcmp(modelTyp,"hetoni") )
	return new hetoni(*((hetoni*)sourceModel));
    if( !strcmp(modelTyp,"nicole") )
	return new nicole(*((nicole*)sourceModel));
    if( !strcmp(modelTyp,"growth_model") )
	return new growth_model(*((growth_model*)sourceModel));
    if( !strcmp(modelTyp,"depreciate") )
	return new depreciate(*((depreciate*)sourceModel));
    if( !strcmp(modelTyp,"pasinetti") )
	return new pasinetti(*((pasinetti*)sourceModel));
    if( !strcmp(modelTyp,"rgrowth_model") )
	return new rgrowth_model(*((rgrowth_model*)sourceModel));
    if( !strcmp(modelTyp,"jan_model") )
	return new jan_model(*((jan_model*)sourceModel));
    if( !strcmp(modelTyp,"rSolow") )
	return new rSolow(*((rSolow*)sourceModel));
    if( !strcmp(modelTyp,"RBC_delta_1") )
	return new RBC_delta_1(*((RBC_delta_1*)sourceModel));
    if( !strcmp(modelTyp,"dynofin_rmv") )
	return new dynofin_rmv(*((dynofin_rmv*)sourceModel));	
    if( !strcmp(modelTyp,"armax_eindim") )
        return new armax_eindim(*((armax_eindim*)sourceModel));
    if( !strcmp(modelTyp,"armax") )
	return new armax(*((armax*)sourceModel));
    if( !strcmp(modelTyp,"rOLG_wt") )
	return new rOLG_wt(*((rOLG_wt*)sourceModel));
    if( !strcmp(modelTyp,"dornbusch") )
	return new dornbusch(*((dornbusch*)sourceModel));
    if( !strcmp(modelTyp,"dorn") )
	return new dorn(*((dorn*)sourceModel));
    if( !strcmp(modelTyp,"samuelson1") )
	return new samuelson1(*((samuelson1*)sourceModel));
    if( !strcmp(modelTyp,"frepm") )
	return new frepm(*((frepm*)sourceModel));
    if( !strcmp(modelTyp,"manipulate") )
	return new manipulate(*((manipulate*)sourceModel));
    if( !strcmp(modelTyp,"linFima_basic") )
	return new linFima_basic(*((linFima_basic*)sourceModel));
    if( !strcmp(modelTyp,"linFimaErwRueck") )
	return new linFimaErwRueck(*((linFimaErwRueck*)sourceModel));
    if( !strcmp(modelTyp,"linFima_wage") )
	return new linFima_wage(*((linFima_wage*)sourceModel));
    if( !strcmp(modelTyp,"pureExchange") )
	return new pureExchange(*((pureExchange*)sourceModel));
    if( !strcmp(modelTyp,"martinNeu") )
	return new martinNeu(*((martinNeu*)sourceModel));
    if( !strcmp(modelTyp,"onozaki_et_al") )
	return new onozaki_et_al(*((onozaki_et_al*)sourceModel));
    if( !strcmp(modelTyp,"MerA") )
	return new MerA(*((MerA*)sourceModel));
    if( !strcmp(modelTyp,"Bankintermed") )
	return new Bankintermed(*((Bankintermed*)sourceModel));	
    if( !strcmp(modelTyp,"finanzmarkt_2Dim_1") )
	return new finanzmarkt_2Dim_1(*((finanzmarkt_2Dim_1*)sourceModel));	
    if( !strcmp(modelTyp,"finanzmarkt_wage") )
	return new finanzmarkt_wage(*((finanzmarkt_wage*)sourceModel));	
    if( !strcmp(modelTyp,"finanzmarkt_JanE") )
	return new finanzmarkt_JanE(*((finanzmarkt_JanE*)sourceModel));	
    if( !strcmp(modelTyp,"ExchangeRateModel") )
	return new ExchangeRateModel(*((ExchangeRateModel*)sourceModel));
    if( !strcmp(modelTyp,"ExchangeRateModel2") )
	return new ExchangeRateModel2(*((ExchangeRateModel2*)sourceModel));			
    if( !strcmp(modelTyp,"ELS") )
	return new ELS(*((ELS*)sourceModel));
    if( !strcmp(modelTyp,"demog_model") )
	return new demog_model(*((demog_model*)sourceModel));
    if( !strcmp(modelTyp,"HicksModel") )
	return new HicksModel(*((HicksModel*)sourceModel));
    if( !strcmp(modelTyp,"RBC_basic_model") )
	return new RBC_basic_model(*((RBC_basic_model*)sourceModel));
    if( !strcmp(modelTyp,"Keener") )
	return new Keener(*((Keener*)sourceModel));
    if( !strcmp(modelTyp,"Samuelson_basic") )
	return new Samuelson_basic(*((Samuelson_basic*)sourceModel));	
    if( !strcmp(modelTyp,"Samuelson_ar1") )
	return new Samuelson_ar1(*((Samuelson_ar1*)sourceModel));
	
    if( !strcmp(modelTyp,"test_model") )
	return new test_model(*((test_model*)sourceModel));			
	return NULL;		// not reached
}//getModel(2)
