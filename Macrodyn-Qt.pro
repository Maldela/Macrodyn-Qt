TEMPLATE = app

QT += qml quick widgets

#QMAKE_CXXFLAGS += -Wno-unused-parameter \
#                  -Wno-unused-but-set-variable \
#                  -Wno-unused-variable \
#                  -Wno-unused-function

QMAKE_MAC_SDK = macosx10.9

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
    distri/cauchy_distri.C \
    distri/chi_square_distri.C \
    distri/discrete_distri.C \
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
    eval_expr.C \
    get_expr.C \
    Jobs/3d_attractor.C \
    Jobs/3d_cyclogram.C \
    Jobs/acf_plot.C \
    Jobs/attractor.C \
    Jobs/basin.C \
    Jobs/basinTwoCycles.C \
    Jobs/bif2D.C \
    Jobs/bif2Param.C \
    Jobs/bif3D.C \
    Jobs/bifnParam.C \
    Jobs/cellAttractor.C \
    Jobs/cellBif2D.C \
    Jobs/color_plot.C \
    Jobs/contourline.C \
    Jobs/d_attractor.C \
    Jobs/d_bif2D.C \
    Jobs/d_bifnParam.C \
    Jobs/d_var.C \
    Jobs/density.C \
    Jobs/discreteBif2D.C \
    Jobs/floathisto2d.C \
    Jobs/geometricJob.C \
    Jobs/geometry3D.C \
    Jobs/histo.C \
    Jobs/indicator.C \
    Jobs/job.C \
    Jobs/l_attractor.C \
    Jobs/mapping.C \
    Jobs/max_lyapunov_exp.C \
    Jobs/nParameterAnalysis.C \
    Jobs/pacf_plot.C \
    Jobs/parameterSpace.C \
    Jobs/phase_plot.C \
    Jobs/powerSpec.C \
    Jobs/probability.C \
    Jobs/simpleCellMapping.C \
    Jobs/stateSpaceAnalysis.C \
    Jobs/timeSeriesJob.C \
    Jobs/timeSeriesPlot.C \
    Jobs/timeSeriesPlot_Loop.C \
    Jobs/timeSeriesSet.C \
    Graphics/macrodyngraphicsitem.cpp \
    axes.C \
    hashTab.C \
    MDMap.C \
    logger.cpp \
    Jobs/distributionJob.C \
    block.C \
    simloader.cpp \
    Models/boehmhuels.C

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =/opt/qt5/imports

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
    matrix_neu.h \
    matrix.h \
    distri/cauchy_distri.h \
    distri/chi_square_distri.h \
    distri/discrete_distri.h \
    distri/logger.h \
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
    Random/ranlib.h \
    Jobs/3d_attractor.h \
    Jobs/3d_cyclogram.h \
    Jobs/acf_plot.h \
    Jobs/attractor.h \
    Jobs/basin.h \
    Jobs/basinTwoCycles.h \
    Jobs/bif2D.h \
    Jobs/bif2Param.h \
    Jobs/bif3D.h \
    Jobs/bifnParam.h \
    Jobs/cellAttractor.h \
    Jobs/cellBif2D.h \
    Jobs/color_plot.h \
    Jobs/contourline.h \
    Jobs/d_attractor.h \
    Jobs/d_bif2D.h \
    Jobs/d_bifnParam.h \
    Jobs/d_var.h \
    Jobs/density.h \
    Jobs/discreteBif2D.h \
    Jobs/floathisto2d.h \
    Jobs/geometricJob.h \
    Jobs/geometry3D.h \
    Jobs/histo.h \
    Jobs/indicator.h \
    Jobs/job.h \
    Jobs/l_attractor.h \
    Jobs/mapping.h \
    Jobs/max_lyapunov_exp.h \
    Jobs/nParameterAnalysis.h \
    Jobs/pacf_plot.h \
    Jobs/parameterSpace.h \
    Jobs/phase_plot.h \
    Jobs/powerSpec.h \
    Jobs/probability.h \
    Jobs/simpleCellMapping.h \
    Jobs/stateSpaceAnalysis.h \
    Jobs/timeSeriesJob.h \
    Jobs/timeSeriesPlot.h \
    Jobs/timeSeriesPlot_Loop.h \
    Jobs/timeSeriesSet.h \
    Graphics/macrodyngraphicsitem.h \
    axes.h \
    hashTab.h \
    MDMap.h \
    logger.h \
    Jobs/distributionJob.h \
    block.h \
    simloader.h \
    Models/boehmhuels.h \
    distri/logger.h
