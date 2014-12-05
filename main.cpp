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

const int maxArgc = 4;
int useGraphics = 1;

baseModel* model = NULL;
QString modelTyp;
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

void loadSimulation(const QString& fileName)
{
    QFile file(fileName);
    int divisor = 1;

    if(!file.open(QFile::ReadOnly))
    {
        fatalError("macrodyn.C::loadSimulation  Can't open input file ",fileName);
        return;
    }

    QDataStream stream(&file);

    stream >> modelTyp;
    model = getModel(modelTyp);
    if( !model ) {
        model = getPubModel(modelTyp);
        if( !model)
        {
            fatalError("macrodyn::loadSimulation  Can't allocate model ",modelTyp);
            return;
        }
    }
    model->loadParamset(stream);

    stream >> conBlock.graphTyp;

    stream >> conBlock.xLabel; // at this position commands possible beside xLabel
    if (conBlock.xLabel == "pointsize")
    {
        stream >> timeSeriesPlot::pointsize;
        stream >> conBlock.xLabel;
    }

    if (conBlock.xLabel == "plotlines")
    {
        stream >> i;
        plotLines= new qreal[1+4*i];
        plotLines[0]=i;
        for(i=1;i<=(4*plotLines[0]);i++)
        {
            stream >> plotLines[i];
        }
        stream >> conBlock.xLabel;
    }
    if (conBlock.xLabel == "multiplot")
    {
        stream >> multiplot_num;
        multiplotPtrs = new char*[multiplot_num];
        if( !(multiplotPtrs) )
        fatalError("macrodyn::loadSimulation","Can't create array for multiplot");
        for(int j=0;j<multiplot_num;j++)
        {
            multiplotPtrs[j]=new char[20];
            if( !(*multiplotPtrs) ) fatalError("macrodyn::loadSimulation","Can't create array for multiplot[]");
            stream >> multiplotPtrs[j];
            if(!model->setLabels(multiplotPtrs[j])) fatalError("macrodyn::loadSimulation  Can't find multiplot label ",multiplotPtrs[j]);
        }
        stream >> conBlock.xLabel;
    }
    if (conBlock.xLabel == "zeroline")
    {
        log() << "\n\tx-label zeroline in graphics.C not supported yet !!!\n";
        stream >> conBlock.xLabel;
    }

    stream >> conBlock.xmin >> conBlock.xmax;

    stream >> conBlock.yLabel;
    if (conBlock.yLabel == "zeroline")
    {
        conBlock.zeroy=1; // any positive number; different colors in graphics.C not supported yet
        stream >> conBlock.yLabel;
    }
    stream >> conBlock.ymin >> conBlock.ymax;

    stream >> conBlock.zLabel;
    if (conBlock.zLabel == "zeroline")
    {
        log() << "\n\tz-label zeroline in graphics.C not supported yet !!!\n";
        stream >> conBlock.zLabel;
    }
    stream >> conBlock.zmin >> conBlock.zmax;

//log() << conBlock.xLabel << " " << conBlock.xmin << " " << conBlock.xmax << "\n";
//log() << conBlock.yLabel << " " << conBlock.ymin << " " << conBlock.ymax << "\n";
//log() << conBlock.zLabel << " " << conBlock.zmin << " " << conBlock.zmax << "\n";

    stepX = (conBlock.xmax-conBlock.xmin) / divisor;
    switch( conBlock.graphTyp )
    {
    case PARSPACE:
        stateSpace = new xyRange(stream);
        break;
    case NPARMAP:
        stateSpace = new xyRange(stream);
        xDef = new xyRange(stream);
        yDef = new xyRange(stream);
        break;
    case CONTOURLINE:
    case CONTOURLINE_B:
    case CONTOURLINE_C:
        xDef = new xyRange(stream);
        yDef = new xyRange(stream);
        break;
    case BIFNPARAM:
    case D_BIFNPARAM:
        xDef = new xyRange(stream);
        break;
    case PHASE_PLOT:
        stream >> pp_lag_size;
        break;
    case BIF3D_1PAR:
    case BIF3D_2PAR:
    case ATTRA3D:
        stream >> bif3D_resx >> bif3D_resy >> bif3D_resz;
        stream >> bif3D_dx >> bif3D_dy >> bif3D_dz;
        break;
    case CYCLO3D:
        stateSpace = new xyRange(stream);
        stream >> bif3D_resx >> bif3D_resy >> bif3D_resz;
        stream >> bif3D_dx >> bif3D_dy >> bif3D_dz;
    break;
    case DENSITY_1D_VAR:
        stream >> lag_1;
        break;
    case TSPLOT_LOOP:
        stream >> loop_size;
        break;
    case M_TSPLOT:
    case M_TSPLOT_v:
        int n_values,k;
        stream >> n_values;
        ts_init_values = new double [n_values+1];
        ts_init_values[0] = (qreal) n_values;
        for(k=0; k<n_values; k++)
        {
          stream >> ts_init_values[k+1];
        }
        break;
    case COLOR_PLOT:
    case COLOR_PLOT_B:
    case DISTRIBUTION:
    case PROBABILITY:
        stream >> point_size;
        break;
    case ACF_PLOT:
    case PACF_PLOT:
        lag_1 = long(conBlock.ymin);
        lag_2 = long(conBlock.ymax);
        conBlock.ymin = -1;
        conBlock.ymax = 1;
        conBlock.zmin = conBlock.xmin;
        conBlock.zmax = conBlock.xmax;
        conBlock.xmin = lag_1;
        conBlock.xmax = lag_2;
    break;
    }
}

xyRange* conParam2xyRange(conParam& conBlock, const int dim)
{
    QList<QString> labelList;
    QList<qreal> minList;
    QList<qreal> maxList;
    QList<qint64> resList;
    QList<QColor> zeroList;
    int xDim = 0;
    int yDim = 0;
    int zDim = 0;

    labelList[0] = conBlock.xLabel;
    labelList[1] = conBlock.yLabel;
    labelList[2] = conBlock.zLabel;
    minList[0] = conBlock.xmin;
    minList[1] = conBlock.ymin;
    minList[2] = conBlock.zmin;
    maxList[0] = conBlock.xmax;
    maxList[1] = conBlock.ymax;
    maxList[2] = conBlock.zmax;
    zeroList[0] = conBlock.zerox;
    zeroList[1] = conBlock.zeroy;
    zeroList[2] = conBlock.zeroz;
    resList[0] = xDim;
    resList[1] = yDim;
    resList[2] = zDim;

    return( new xyRange(dim,labelList,minList,maxList,resList,zeroList) );
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<MacrodynGraphicsItem>("MacrodynQML", 1, 0, "MacrodynGraphicsItem");
    qmlRegisterType<Logger>("MacrodynQML", 1, 0, "Logger");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
