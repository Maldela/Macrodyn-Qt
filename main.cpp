#include <QApplication>
#include <QQmlApplicationEngine>

#include "Graphics/macrodyngraphicsitem.h"
#include "logger.h"
#include "sim.h"
#include "axes.h"
#include "block.h"
#include "error.h"
#include "MDMap.h"

#include "Jobs/job.h"
#include "Jobs/attractor.h"
#include "Jobs/basin.h"
#include "Jobs/basinTwoCycles.h"
#include "Jobs/bif2D.h"
#include "Jobs/bif3D.h"
#include "Jobs/bif2Param.h"
#include "Jobs/bifnParam.h"
#include "Jobs/cellAttractor.h"
#include "Jobs/cellBif2D.h"
#include "Jobs/discreteBif2D.h"
#include "Jobs/geometricJob.h"
#include "Jobs/geometry3D.h"
#include "Jobs/nParameterAnalysis.h"
#include "Jobs/parameterSpace.h"
#include "Jobs/powerSpec.h"
#include "Jobs/simpleCellMapping.h"
#include "Jobs/stateSpaceAnalysis.h"
#include "Jobs/timeSeriesJob.h"
#include "Jobs/timeSeriesPlot.h"
#include "Jobs/timeSeriesPlot_Loop.h"
#include "Jobs/max_lyapunov_exp.h"
#include "Jobs/d_bif2D.h"
#include "Jobs/density.h"
#include "Jobs/indicator.h"
#include "Jobs/d_attractor.h"
#include "Jobs/l_attractor.h"
#include "Jobs/phase_plot.h"
#include "Jobs/color_plot.h"
#include "Jobs/contourline.h"
#include "Jobs/probability.h"
#include "Jobs/distributionJob.h"
#include "Jobs/acf_plot.h"
#include "Jobs/pacf_plot.h"
#include "Jobs/mapping.h"	// unfinished
#include "Jobs/timeSeriesSet.h"
#include "Jobs/d_bifnParam.h"
#include "Jobs/3d_cyclogram.h"
#include "Jobs/3d_attractor.h"

#include "Models/getModel.h"
#include "PublicModels/getPubModel.h"

const int maxArgc=4;
int useGraphics=1;

baseModel* model = NULL;
char modelTyp[40];
qreal stepX;

conParam conBlock;
//TODO graphics *graph = NULL;
uint idx = 0;
xyRange *stateSpace = NULL;
xyRange *xDef = NULL;
xyRange *yDef = NULL;

int whichColorMap = 0;
qreal *plotLines = NULL;
int i; //counter

// additional parameters for certain jobs

qint64 pp_lag_size = 1;		// phase_plot lag size k
qreal *ts_init_values = NULL;	// ts_plot initial values
int multiplot_num;		// number of additional variables to plot
char ** multiplotPtrs = NULL;	// addresses of additional variables
int point_size;			// size of plotted points for color_plot_var
int loop_size;			// size of loop for timeSeriesPlot_Loop
qint64 bif3D_resx, bif3D_resy, bif3D_resz;
qint64 bif3D_dx, bif3D_dy, bif3D_dz;
qint64 lag_1, lag_2;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<MacrodynGraphicsItem>("MacrodynQML", 1, 0, "MacrodynGraphicsItem");
    qmlRegisterType<Logger>("MacrodynQML", 1, 0, "Logger");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
