TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    Models/adaptExp.h \
    Models/affine1.h \
    Models/affine2.h \
    Models/affine3.h \
    Models/armax.h \
    Models/armax_eindim.h \
    Models/Bankintermed.h \
    Models/Bankintermed_old.h \
    Models/baseModel.h \
    Models/capOlg.h \
    Models/cobweb_g_predictor.h \
    Models/constantLag.h \
    Models/defaultModel.h \
    Models/defbudba.h \
    Models/demog_model.h \
    Models/det_var.h \
    Models/distribution.h \
    Models/dorn.h \
    Models/dorn_1.h \
    Models/dornbusch.h \
    Models/Duopol.h \
    Models/dynofin_rmv.h \
    Models/ELS.h \
    Models/EnRAss.h \
    Models/EnRAssH.h \
    Models/ExchangeDefault.h \
    Models/ExchangeRateModel.h \
    Models/ExchangeRateModel2.h \
    Models/ExchangeRateModel_save.h \
    Models/ExchangeSave.h \
    Models/excOlg.h \
    Models/finanzmarkt_2Dim.h \
    Models/finanzmarkt_2Dim_1.h \
    Models/finanzmarkt_gruppenswitch.h \
    Models/finanzmarkt_Jan.h \
    Models/finanzmarkt_JanE.h \
    Models/finanzmarkt_JanE_Andi.h \
    Models/finanzmarkt_old.h \
    Models/finanzmarkt_wage.h \
    Models/frepm.h \
    Models/geoExp.h \
    Models/getModel.h \
    Models/growth_model.h \
    Models/henon.h \
    Models/hetoni.h \
    Models/HicksModel.h \
    Models/jan_model.h \
    Models/Keener.h \
    Models/laborMarketFirst.h \
    Models/laborMarketFirstTax.h \
    Models/leo.h \
    Models/linDiff.h \
    Models/linFima_basic.h \
    Models/linFima_wage.h \
    Models/linFimaErwRueck.h \
    Models/logiDelay.h \
    Models/logistic.h \
    Models/manipulate.h \
    Models/markov_chain.h \
    Models/martin.h \
    Models/martinNeu.h \
    Models/MerA.h \
    Models/michael.h \
    Models/milano.h \
    Models/milano2.h \
    Models/milano3.h \
    Models/milano_bcw.h \
    Models/models2D.h \
    Models/Monopol.h \
    Models/newMacro.h \
    Models/nicole.h \
    Models/OlgRat.h \
    Models/onozaki_et_al.h \
    Models/pf_type.h \
    Models/pureExchange.h \
    Models/radio.h \
    Models/rand_var.h \
    Models/RBC_basic_model.h \
    Models/rOLG_wt.h \
    Models/rSolow.h \
    Models/samuelson1.h \
    Models/Samuelson_basic.h \
    Models/Samuelson_basicAlt.h \
    Models/test_model.h \
    Models/toni2.h \
    Models/toniadp.h \
    Models/toniadp2.h \
    Models/tonicapm.h \
    Models/tonicapmLag.h \
    Models/toniLag.h \
    Models/toniRV.h \
    PublicModels/baseModel.h \
    PublicModels/cobweb.h \
    PublicModels/defaultModel.h \
    PublicModels/getPubModel.h \
    PublicModels/models2D.h \
    PublicModels/perturbedDelayedLogisticMap.h \
    PublicModels/rand_var.h
