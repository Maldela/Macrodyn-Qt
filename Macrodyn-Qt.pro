TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    Models/adaptExp.C \
    Models/affine1.C \
    Models/affine2.C \
    Models/affine3.C \
    Models/armax.C \
    Models/armax_eindim.C \
    Models/Bankintermed.C \
    Models/baseModel.C \
    Models/capOlg.C \
    Models/cobweb_g_predictor.C \
    Models/constantLag.C \
    Models/defbudba.C \
    Models/demog_model.C \
    Models/det_var.C \
    Models/distribution.C \
    Models/dorn.C \
    Models/dornbusch.C \
    Models/Duopol.C \
    Models/dynofin_rmv.C \
    Models/ELS.C \
    Models/EnRAss.C \
    Models/EnRAssH.C \
    Models/ExchangeRateModel.C \
    Models/ExchangeRateModel2.C \
    Models/excOlg.C \
    Models/finanzmarkt_2Dim.C \
    Models/finanzmarkt_2Dim_1.C \
    Models/finanzmarkt_gruppenswitch.C \
    Models/finanzmarkt_Jan.C \
    Models/finanzmarkt_JanE_Andi.C \
    Models/finanzmarkt_wage.C \
    Models/frepm.C \
    Models/geoExp.C \
    Models/getModel.C \
    Models/growth_model.C \
    Models/henon.C \
    Models/hetoni.C \
    Models/HicksModel.C \
    Models/jan_model.C \
    Models/Keener.C \
    Models/laborMarketFirst.C \
    Models/laborMarketFirstTax.C \
    Models/leo.C \
    Models/linDiff.C \
    Models/linFima_basic.C \
    Models/linFima_wage.C \
    Models/linFimaErwRueck.C \
    Models/logiDelay.C \
    Models/logistic.C \
    Models/manipulate.C \
    Models/markov_chain.C \
    Models/martin.C \
    Models/martinNeu.C \
    Models/MerA.C \
    Models/michael.C \
    Models/milano.C \
    Models/milano2.C \
    Models/milano3.C \
    Models/milano_bcw.C \
    Models/models2D.C \
    Models/Monopol.C \
    Models/newMacro.C \
    Models/nicole.C \
    Models/OlgRat.C \
    Models/onozaki_et_al.C \
    Models/pf_type.C \
    Models/pureExchange.C \
    Models/radio.C \
    Models/rand_var.C \
    Models/RBC_basic_model.C \
    Models/rOLG_wt.C \
    Models/rSolow.C \
    Models/samuelson1.C \
    Models/Samuelson_basic.C \
    Models/test_model.C \
    Models/toni2.C \
    Models/toniadp.C \
    Models/toniadp2.C \
    Models/tonicapm.C \
    Models/tonicapmLag.C \
    Models/toniLag.C \
    Models/toniRV.C \
    PublicModels/cobweb.C \
    PublicModels/defaultModel.C \
    PublicModels/getPubModel.C \
    PublicModels/perturbedDelayedLogisticMap.C \
    matrix_neu.C \
    matrix.C \
    log.cpp \
    distri/cauchy_distri.C \
    distri/chi_square_distri.C \
    distri/discrete_distri.C \
    distri/error.C \
    distri/exponential_distri.C \
    distri/finite_distri.C \
    distri/gamma_distri.C \
    distri/geometric_distri.C \
    distri/multi_normal_distri.C \
    distri/normal_distri.C \
    distri/pareto_distri.C \
    distri/prob_distri.C \
    distri/random_bits.C \
    distri/uniform_distri.C \
    error.C \
    eval_expr.C \
    get_expr.C \
    strnchr.C

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
    PublicModels/rand_var.h \
    sim.h \
    error.h \
    matrix_neu.h \
    matrix.h \
    log.h \
    distri/cauchy_distri.h \
    distri/chi_square_distri.h \
    distri/discrete_distri.h \
    distri/error.h \
    distri/exponential_distri.h \
    distri/finite_distri.h \
    distri/gamma_distri.h \
    distri/geometric_distri.h \
    distri/machine.h \
    distri/multi_normal_distri.h \
    distri/normal_distri.h \
    distri/pareto_distri.h \
    distri/prob_distri.h \
    distri/random_bits.h \
    distri/uniform_distri.h \
    eval_expr.h \
    get_expr.h \
    strnchr.h \
    Random/ranlib.h