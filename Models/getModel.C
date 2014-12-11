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
#include "finanzmarkt_JanE_Andi.h"
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
baseModel* getModel(const QString& modelTyp)
{
    qDebug() << "get model: " << modelTyp;
    if (modelTyp == "radio")
	return new radio;
    if (modelTyp == "newMacro")
	return new newMacro;
    if (modelTyp == "geoExp")
	return new geoExp;
    if (modelTyp == "adaptExp")
	return new adaptExp;
    if (modelTyp == "affine1")
	return new affine1;	
    if (modelTyp == "affine2")
	return new affine2;
    if (modelTyp == "affine3")
	return new affine3;
    if (modelTyp == "constantLag")
	return new constantLag;
    if (modelTyp == "logistic")
	return new logistic;
    if (modelTyp == "rlogistic")
	return new rlogistic;
    if (modelTyp == "henon")
	return new henon;
    if (modelTyp == "quadHenon")
	return new quadHenon;
    if (modelTyp == "logiDelay")
	return new logiDelay;
    if (modelTyp == "logLogDelay")
	return new logLogDelay;
    if (modelTyp == "linDiff")
	return new linDiff;
    if (modelTyp == "martin")
	return new martin;
    if (modelTyp == "martin2")
	return new martin2;
    if (modelTyp == "martin3")
	return new martin3;
    if (modelTyp == "capOlg")
	return new capOlg;
    if (modelTyp == "capOlgAdapt")
	return new capOlgAdapt;
    if (modelTyp == "capOlgGeoExp")
	return new capOlgGeoExp;
    if (modelTyp == "excOlg")
	return new excOlg;
    if (modelTyp == "excOlgAdapt")
	return new excOlgAdapt;
    if (modelTyp == "excOlgGeoExp")
	return new excOlgGeoExp;
    if (modelTyp == "laborMarketFirst")
	return new laborMarketFirst;
    if (modelTyp == "leo")
	return new leo;
    if (modelTyp == "laborMarketFirstTax")
	return new laborMarketFirstTax;
    if (modelTyp == "rlaborMarketFirstTax")
    	return new rlaborMarketFirstTax;
    if (modelTyp == "martin4")
	return new martin4;
    if (modelTyp == "OlgRat")
	return new OlgRat;
    if (modelTyp == "OlgRat2")
	return new OlgRat2;
    if (modelTyp == "defbudba")
	return new defbudba;
    if (modelTyp == "Monopol")
	return new Monopol;
    if (modelTyp == "Duopol")
	return new Duopol;
    if (modelTyp == "Duopol2")
	return new Duopol2;
    if (modelTyp == "cobweb_g_predictor")
    return new cobweb_g_predictor;
    if (modelTyp == "toniRV")
    return new toniRV;
    if (modelTyp == "tonicapm")
    return new tonicapm;
    if (modelTyp == "tonicapmLag")
    return new tonicapmLag;
    if (modelTyp == "toniLag")
    return new toniLag;
    if (modelTyp == "toniadp")
    return new toniadp;
    if (modelTyp == "toniadp2")
    return new toniadp;
    if (modelTyp == "toni2")
    return new toni2;
    if (modelTyp == "hetoni")
	return new hetoni;
    if (modelTyp == "growth_model")
	return new growth_model;
    if (modelTyp == "rgrowth_model")
	return new rgrowth_model;
    if (modelTyp == "depreciate")
	return new depreciate;
    if (modelTyp == "pasinetti")
	return new pasinetti;
    if (modelTyp == "nicole")
	return new nicole;
    if (modelTyp == "jan_model")
	return new jan_model;
    if (modelTyp == "rSolow")
	return new rSolow;
    if (modelTyp == "RBC_delta_1")
	return new RBC_delta_1;    
    if (modelTyp == "dynofin_rmv")
	return new dynofin_rmv;
    if (modelTyp == "armax_eindim")
    return new armax_eindim;
    if (modelTyp == "armax")
    return new armax;
    if (modelTyp == "rOLG_wt")
	return new rOLG_wt;
    if (modelTyp == "dornbusch")
	return new dornbusch;
    if (modelTyp == "dorn")
	return new dorn;
    if (modelTyp == "samuelson1")
	return new samuelson1;
    if (modelTyp == "frepm")
	return new frepm;
    if (modelTyp == "manipulate")
	return new manipulate;
    if (modelTyp == "linFima_basic")
	return new linFima_basic;
    if (modelTyp == "linFimaErwRueck")
	return new linFimaErwRueck;
    if (modelTyp == "linFima_wage")
	return new linFima_wage;
    if (modelTyp == "pureExchange")
	return new pureExchange;
    if (modelTyp == "martinNeu")
	return new martinNeu;
    if (modelTyp == "onozaki_et_al")
	return new onozaki_et_al;
    if (modelTyp == "MerA")
	return new MerA;
    if (modelTyp == "Bankintermed")
	return new Bankintermed;
    if (modelTyp == "finanzmarkt_2Dim_1")
	return new finanzmarkt_2Dim_1;
    if (modelTyp == "finanzmarkt_wage")
	return new finanzmarkt_wage;
    if (modelTyp == "finanzmarkt_JanE")
	return new finanzmarkt_JanE;
    if (modelTyp == "ExchangeRateModel")
	return new ExchangeRateModel;
    if (modelTyp == "ExchangeRateModel2")
	return new ExchangeRateModel2;	
    if (modelTyp == "ELS")
	return new ELS;
    if (modelTyp == "demog_model")
	return new demog_model;	
    if (modelTyp == "HicksModel")
	return new HicksModel;
    if (modelTyp == "RBC_basic_model")
	return new RBC_basic_model;
    if (modelTyp == "Keener")
	return new Keener;
    if (modelTyp == "Samuelson_basic")
	return new Samuelson_basic;
    if (modelTyp == "Samuelson_ar1")
	return new Samuelson_ar1;
	
    if (modelTyp == "test_model")
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
baseModel* getModel(const QString& modelTyp, const baseModel* sourceModel)
{
    qDebug() << "get model: " << modelTyp;
    if (modelTyp == "radio")
	return new radio(*((radio*)sourceModel));
    if (modelTyp == "newMacro")
	return new newMacro(*((newMacro*)sourceModel));
    if (modelTyp == "logistic")
	return new logistic(*((logistic*)sourceModel));
    if (modelTyp == "rlogistic")
	return new rlogistic(*((rlogistic*)sourceModel));
    if (modelTyp == "geoExp")
	return new geoExp(*((geoExp*)sourceModel));
    if (modelTyp == "adaptExp")
	return new adaptExp(*((adaptExp*)sourceModel));
    if (modelTyp == "affine1")
	return new affine1(*((affine1*)sourceModel));	
    if (modelTyp == "affine2")
	return new affine2(*((affine2*)sourceModel));
    if (modelTyp == "affine3")
	return new affine3(*((affine3*)sourceModel));
    if (modelTyp == "constantLag")
	return new constantLag(*((constantLag*)sourceModel));
    if (modelTyp == "henon")
	return new henon(*((henon*)sourceModel));
    if (modelTyp == "quadHenon")
	return new quadHenon(*((quadHenon*)sourceModel));
    if (modelTyp == "logiDelay")
	return new logiDelay(*((logiDelay*)sourceModel));
    if (modelTyp == "logLogDelay")
	return new logLogDelay(*((logLogDelay*)sourceModel));
    if (modelTyp == "linDiff")
	return new linDiff(*((linDiff*)sourceModel));
    if (modelTyp == "martin")
	return new martin(*((martin*)sourceModel));
    if (modelTyp == "martin2")
	return new martin2(*((martin2*)sourceModel));
    if (modelTyp == "martin3")
	return new martin3(*((martin3*)sourceModel));
    if (modelTyp == "capOlg")
	return new capOlg(*((capOlg*)sourceModel));
    if (modelTyp == "capOlgAdapt")
	return new capOlgAdapt(*((capOlgAdapt*)sourceModel));
    if (modelTyp == "capOlgGeoExp")
	return new capOlgGeoExp(*((capOlgGeoExp*)sourceModel));
    if (modelTyp == "excOlg")
	return new excOlg(*((excOlg*)sourceModel));
    if (modelTyp == "excOlgAdapt")
	return new excOlgAdapt(*((excOlgAdapt*)sourceModel));
    if (modelTyp == "excOlgGeoExp")
	return new excOlgGeoExp(*((excOlgGeoExp*)sourceModel));
    if (modelTyp == "laborMarketFirst")
	return new laborMarketFirst(*((laborMarketFirst*)sourceModel));
    if (modelTyp == "leo")
	return new leo(*((leo*)sourceModel));
    if (modelTyp == "laborMarketFirstTax")
	return new laborMarketFirstTax(*((laborMarketFirstTax*)sourceModel));
    if (modelTyp == "rlaborMarketFirstTax")
        return new rlaborMarketFirstTax(*((rlaborMarketFirstTax*)sourceModel));
    if (modelTyp == "martin4")
	return new martin4(*((martin4*)sourceModel));
    if (modelTyp == "OlgRat")
	return new OlgRat(*((OlgRat*)sourceModel));
    if (modelTyp == "OlgRat2")
	return new OlgRat2(*((OlgRat2*)sourceModel));
    if (modelTyp == "defbudba")
	return new defbudba(*((defbudba*)sourceModel));
    if (modelTyp == "Monopol")
	return new Monopol(*((Monopol*)sourceModel));
    if (modelTyp == "Duopol")
	return new Duopol(*((Duopol*)sourceModel));
    if (modelTyp == "Duopol2")
	return new Duopol2(*((Duopol2*)sourceModel));
    if (modelTyp == "cobweb_g_predictor")
        return new cobweb_g_predictor(*((cobweb_g_predictor*)sourceModel));
    if (modelTyp == "toniRV")
        return new toniRV(*((toniRV*)sourceModel));
    if (modelTyp == "tonicapm")
      return new tonicapm(*((tonicapm*)sourceModel));
    if (modelTyp == "tonicapmLag")
      return new tonicapmLag(*((tonicapmLag*)sourceModel));
    if (modelTyp == "toniLag")
	return new toniLag(*((toniLag*)sourceModel));
    if (modelTyp == "toniadp")
	return new toniadp(*((toniadp*)sourceModel));
    if (modelTyp == "toniadp2")
	return new toniadp2(*((toniadp2*)sourceModel));
    if (modelTyp == "toni2")
	return new toni2(*((toni2*)sourceModel));
    if (modelTyp == "hetoni")
	return new hetoni(*((hetoni*)sourceModel));
    if (modelTyp == "nicole")
	return new nicole(*((nicole*)sourceModel));
    if (modelTyp == "growth_model")
	return new growth_model(*((growth_model*)sourceModel));
    if (modelTyp == "depreciate")
	return new depreciate(*((depreciate*)sourceModel));
    if (modelTyp == "pasinetti")
	return new pasinetti(*((pasinetti*)sourceModel));
    if (modelTyp == "rgrowth_model")
	return new rgrowth_model(*((rgrowth_model*)sourceModel));
    if (modelTyp == "jan_model")
	return new jan_model(*((jan_model*)sourceModel));
    if (modelTyp == "rSolow")
	return new rSolow(*((rSolow*)sourceModel));
    if (modelTyp == "RBC_delta_1")
	return new RBC_delta_1(*((RBC_delta_1*)sourceModel));
    if (modelTyp == "dynofin_rmv")
	return new dynofin_rmv(*((dynofin_rmv*)sourceModel));	
    if (modelTyp == "armax_eindim")
        return new armax_eindim(*((armax_eindim*)sourceModel));
    if (modelTyp == "armax")
    return new armax(*((armax*)sourceModel));
    if (modelTyp == "rOLG_wt")
	return new rOLG_wt(*((rOLG_wt*)sourceModel));
    if (modelTyp == "dornbusch")
	return new dornbusch(*((dornbusch*)sourceModel));
    if (modelTyp == "dorn")
	return new dorn(*((dorn*)sourceModel));
    if (modelTyp == "samuelson1")
	return new samuelson1(*((samuelson1*)sourceModel));
    if (modelTyp == "frepm")
	return new frepm(*((frepm*)sourceModel));
    if (modelTyp == "manipulate")
	return new manipulate(*((manipulate*)sourceModel));
    if (modelTyp == "linFima_basic")
	return new linFima_basic(*((linFima_basic*)sourceModel));
    if (modelTyp == "linFimaErwRueck")
	return new linFimaErwRueck(*((linFimaErwRueck*)sourceModel));
    if (modelTyp == "linFima_wage")
	return new linFima_wage(*((linFima_wage*)sourceModel));
    if (modelTyp == "pureExchange")
	return new pureExchange(*((pureExchange*)sourceModel));
    if (modelTyp == "martinNeu")
	return new martinNeu(*((martinNeu*)sourceModel));
    if (modelTyp == "onozaki_et_al")
	return new onozaki_et_al(*((onozaki_et_al*)sourceModel));
    if (modelTyp == "MerA")
	return new MerA(*((MerA*)sourceModel));
    if (modelTyp == "Bankintermed")
	return new Bankintermed(*((Bankintermed*)sourceModel));	
    if (modelTyp == "finanzmarkt_2Dim_1")
	return new finanzmarkt_2Dim_1(*((finanzmarkt_2Dim_1*)sourceModel));	
    if (modelTyp == "finanzmarkt_wage")
	return new finanzmarkt_wage(*((finanzmarkt_wage*)sourceModel));	
    if (modelTyp == "finanzmarkt_JanE")
	return new finanzmarkt_JanE(*((finanzmarkt_JanE*)sourceModel));	
    if (modelTyp == "ExchangeRateModel")
	return new ExchangeRateModel(*((ExchangeRateModel*)sourceModel));
    if (modelTyp == "ExchangeRateModel2")
	return new ExchangeRateModel2(*((ExchangeRateModel2*)sourceModel));			
    if (modelTyp == "ELS")
	return new ELS(*((ELS*)sourceModel));
    if (modelTyp == "demog_model")
	return new demog_model(*((demog_model*)sourceModel));
    if (modelTyp == "HicksModel")
	return new HicksModel(*((HicksModel*)sourceModel));
    if (modelTyp == "RBC_basic_model")
	return new RBC_basic_model(*((RBC_basic_model*)sourceModel));
    if (modelTyp == "Keener")
	return new Keener(*((Keener*)sourceModel));
    if (modelTyp == "Samuelson_basic")
	return new Samuelson_basic(*((Samuelson_basic*)sourceModel));	
    if (modelTyp == "Samuelson_ar1")
	return new Samuelson_ar1(*((Samuelson_ar1*)sourceModel));
	
    if (modelTyp == "test_model")
	return new test_model(*((test_model*)sourceModel));			
	return NULL;		// not reached
}//getModel(2)
