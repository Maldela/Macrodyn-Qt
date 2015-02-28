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

#include "../logger.h"

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
}
