#include "simloader.h"
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

#include "logger.h"
#include "sim.h"
#include "axes.h"
#include "error.h"
#include "MDMap.h"

const int qMaxArgc = 4;

qreal stepX;

//TODO
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


xyRange* conParam2xyRange(const conParam& m_conBlock, const int dim)
{
    QList<QString> labelList;
    QList<qreal> qMinList;
    QList<qreal> qMaxList;
    QList<qint64> resList;
    QList<QColor> zeroList;
    int xDim = 0;
    int yDim = 0;
    int zDim = 0;

    labelList << m_conBlock.xLabel << m_conBlock.yLabel << m_conBlock.zLabel;
    qMinList << m_conBlock.xmin << m_conBlock.ymin << m_conBlock.zmin;
    qMaxList << m_conBlock.xmax << m_conBlock.ymax << m_conBlock.zmax;
    zeroList << m_conBlock.zerox << m_conBlock.zeroy << m_conBlock.zeroz;
    resList << xDim << yDim << zDim;

    return( new xyRange(dim,labelList,qMinList,qMaxList,resList,zeroList) );
}


SimLoader::SimLoader(QObject *parent) : QObject(parent)
{
    m_modelPointer = NULL;
    m_graph = NULL;
}

SimLoader::~SimLoader()
{
    if (m_modelPointer) delete m_modelPointer;
}

void SimLoader::loadSimulationfromFile(const QString& fileName)
{
    QFile file(fileName);
    int divisor = 1;

    if(!file.open(QFile::ReadOnly))
    {
        log() << "macrodyn.C::loadSimulation  Can't open input file " << fileName;
        return;
    }

    QTextStream stream(&file);
    QString model;
    stream >> model;
    setModel(model);

    if (m_modelPointer)
    {
        log() << "Model:" << m_modelName;
    }
    else
    {
        log() << "Could not load model " << m_modelName;
        return;
    }

    m_modelPointer->loadParamset(stream);


    stream >> m_conBlock.graphTyp;
    log() << "Graph Typ:" << m_conBlock.graphTyp;

    stream >> m_conBlock.xLabel; // at this position commands possible beside xLabel
             ;
    if (m_conBlock.xLabel == "pointsize")
    {
        stream >> timeSeriesPlot::pointsize;
        stream >> m_conBlock.xLabel;
    }

    if (m_conBlock.xLabel == "plotlines")
    {
        stream >> i;
        plotLines= new qreal[1+4*i];
        plotLines[0]=i;
        for(i=1;i<=(4*plotLines[0]);i++)
        {
            stream >> plotLines[i];
        }
        stream >> m_conBlock.xLabel;
    }
    if (m_conBlock.xLabel == "multiplot")
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
            if(!m_modelPointer->setLabels(multiplotPtrs[j])) fatalError("macrodyn::loadSimulation  Can't find multiplot label ",multiplotPtrs[j]);
        }
        stream >> m_conBlock.xLabel;
    }
    if (m_conBlock.xLabel == "zeroline")
    {
        log() << "\n\tx-label zeroline in graphics.C not supported yet !!!\n";
        stream >> m_conBlock.xLabel;
    }

    stream >> m_conBlock.xmin >> m_conBlock.xmax;

    stream >> m_conBlock.yLabel;

    if (m_conBlock.yLabel == "zeroline")
    {
        m_conBlock.zeroy=1; // any positive number; different colors in graphics.C not supported yet
        stream >> m_conBlock.yLabel;
    }
    stream >> m_conBlock.ymin >> m_conBlock.ymax;

    stream >> m_conBlock.zLabel;

    if (m_conBlock.zLabel == "zeroline")
    {
        log() << "\n\tz-label zeroline in graphics.C not supported yet !!!\n";
        stream >> m_conBlock.zLabel;
    }
    stream >> m_conBlock.zmin >> m_conBlock.zmax;

    log() << "X:" << m_conBlock.xLabel << "from" << m_conBlock.xmin << "to" << m_conBlock.xmax;
    log() << "Y:" << m_conBlock.yLabel << "from" << m_conBlock.ymin << "to" << m_conBlock.ymax;
    log() << "Z:" << m_conBlock.zLabel << "from" << m_conBlock.zmin << "to" << m_conBlock.zmax;

    stepX = (m_conBlock.xmax-m_conBlock.xmin) / divisor;
    switch( m_conBlock.graphTyp )
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
        lag_1 = long(m_conBlock.ymin);
        lag_2 = long(m_conBlock.ymax);
        m_conBlock.ymin = -1;
        m_conBlock.ymax = 1;
        m_conBlock.zmin = m_conBlock.xmin;
        m_conBlock.zmax = m_conBlock.xmax;
        m_conBlock.xmin = lag_1;
        m_conBlock.xmax = lag_2;
    break;
    }
}

void SimLoader::runSimulation()
{
    qDebug() << "Run simulation";
    Job *runJob;
    qDebug() << "New RunJob*";

    xyRange* axes = conParam2xyRange(m_conBlock, 2);
    window wind = HANNING; // only case Power, leave it here

    qDebug() << "switch reached...\n";
    switch( m_conBlock.graphTyp )
    {
        case ATTRA:
            runJob = new attractor(m_modelPointer,*axes,m_graph);
            break;
        case D_ATTRA:
            runJob = new d_attractor(m_modelPointer,*axes,m_graph);
            break;
        case L_ATTRA:
            runJob = new l_attractor(m_modelPointer,*axes,m_graph,timeSeriesPlot::pointsize);
            break;
        case PHASE_PLOT:
            runJob = new phase_plot(pp_lag_size,m_modelPointer,*axes,m_graph,timeSeriesPlot::pointsize);
            break;
        case CELLATTRA:
            runJob = new cellAttractor(m_modelPointer,*axes,m_graph);
            break;
        case BIF2D:
            runJob = new bif2D(m_modelPointer,*axes,m_graph);
            runJob->setStepX(stepX);
            break;
        case BIF3D_1PAR:
            if ( axes )
                delete axes;
            axes=conParam2xyRange(m_conBlock,3);
            axes->res[0]=bif3D_resx;
            axes->res[1]=bif3D_resy;
            axes->res[2]=bif3D_resz;
            runJob = new bif3D_1par(m_modelPointer,*axes,m_graph,bif3D_dx,
            bif3D_dy,bif3D_dz);
            break;
        case BIF3D_2PAR:
            if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
            axes->res[0]=bif3D_resx;
            axes->res[1]=bif3D_resy;
            axes->res[2]=bif3D_resz;
            runJob = new bif3D_2par(m_modelPointer,*axes,m_graph,bif3D_dx,
            bif3D_dy,bif3D_dz);
            break;
        case CYCLO3D:
            if ( axes )
                delete axes;
            axes=conParam2xyRange(m_conBlock,3);
            axes->res[0]=bif3D_resx;
            axes->res[1]=bif3D_resy;
            axes->res[2]=bif3D_resz;
            runJob = new cyclogram_3d(m_modelPointer,*axes,*stateSpace,m_graph,bif3D_dx,
            bif3D_dy,bif3D_dz);
            break;
        case ATTRA3D:
            if ( axes )
                delete axes;
            axes=conParam2xyRange(m_conBlock,3);
            axes->res[0]=bif3D_resx;
            axes->res[1]=bif3D_resy;
            axes->res[2]=bif3D_resz;
            runJob = new attractor_3d(m_modelPointer,*axes,m_graph);
            break;
        case D_BIF2D:
            runJob = new d_bif2D(m_modelPointer,*axes,m_graph);
            runJob->setStepX(stepX);
            break;
        case D_BIF2D_F2:
            runJob = new d_bif2D_f2(m_modelPointer,*axes,m_graph);
            runJob->setStepX(stepX);
            break;
        case D_BIF2D_F4:
            runJob = new d_bif2D_f4(m_modelPointer,*axes,m_graph);
            runJob->setStepX(stepX);
            break;
        case DENSITY_1D:
            runJob = new density_1d(m_modelPointer,*axes,m_graph);
            break;
        case DENSITY_1D_1P:
                if( !m_graph )
            break;
            if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
            axes->res[2]=10;
            runJob = new density_1d_1p(m_modelPointer,*axes,m_graph);
            break;
        case DENSITY_1D_VAR:
            if ( axes ) delete axes;
            axes=conParam2xyRange(m_conBlock,3);
            axes->res[2]=lag_1;
            runJob = new density_1d_var(m_modelPointer,*axes,m_graph);
            break;
        case INDICATOR_2D:
                if( !m_graph )
            break;
            if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);	// what a pity
            runJob = new indicator_2d(m_modelPointer,*axes,m_graph);
            break;
        case DISCBIF2D:
                axes->res[0]=(short)(axes->max[0]-axes->min[0]+1);
            runJob = new discreteBif2D(m_modelPointer,*axes,m_graph);
            runJob->setStepX(stepX);
            break;
        case CELLBIF2D:
            runJob = new cellBif2D(m_modelPointer,*axes,m_graph);
            break;
        case PARSPACE:
            runJob = new parameterSpace(m_modelPointer,*axes,*stateSpace,m_graph);
            runJob->setStepX(stepX);
            break;
        case MAX_LAYP_EXP_1D:
            runJob = new max_lyapunov_exp_1d(m_modelPointer,*axes,m_graph);
            runJob->setStepX(stepX);
            break;
        case MAX_LAYP_EXP_2D:
            runJob = new max_lyapunov_exp_2d(m_modelPointer,*axes,m_graph);
            runJob->setStepX(stepX);
            break;
        case MAX_LAYP_EXP_T:
            runJob = new max_lyapunov_exp_t(m_modelPointer,*axes,m_graph);
            runJob->setStepX(stepX);
            break;
        case NPARMAP:
                runJob = new nParameterAnalysis(m_modelPointer,*axes,*stateSpace,
                          *xDef,*yDef,m_graph);
            runJob->setStepX(stepX);
            break;
        case BIFNPARAM:
                runJob = new bifnParam(m_modelPointer,*axes,
                          *xDef,m_graph);
            runJob->setStepX(stepX);
            break;
        case D_BIFNPARAM:
            runJob = new d_bifnParam(m_modelPointer, *axes, *xDef, m_graph);
            runJob->setStepX(stepX);
            break;
        case SIMPLECELL:
            runJob = new simpleCellMapping(m_modelPointer,*axes,m_graph);
            break;
        case STATEANAL:
            runJob = new stateSpaceAnalysis(m_modelPointer,*axes,m_graph);
            break;
        case BIF2PAR:
                if( !m_graph )
            break;
            if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
            axes->res[2]=10;
            runJob = new bif2Param(m_modelPointer,*axes,m_graph);
            break;
        case BASIN:
            runJob = new basin(m_modelPointer,*axes,"cycle.asc",m_graph);
            runJob->setStepX(stepX);
            break;
        case BASIN2:
            runJob = new basinTwoCycles(m_modelPointer,*axes,"cycle1.asc", "cycle2.asc",m_graph);
            runJob->setStepX(stepX);
            break;
        case HORSE: break;
        /**********************************
                graph=new graph2D(m_conBlock.xmin,m_conBlock.xmax,
                      m_conBlock.ymin,m_conBlock.ymax);
            if( graph && !m_conBlock.createPrint ) {
            xDim=graph->sizeX();
            yDim=graph->sizeY();
            }
            axes=new xyRange(m_conBlock.xLabel,m_conBlock.xmin,
                     m_conBlock.xmax,xDim,m_conBlock.yLabel,
                     m_conBlock.ymin,m_conBlock.ymax,yDim);
            printDevice=createPrinter(*axes);
            runJob = new horseshoe(m_modelPointer,*axes,m_graph);
            break;
         ************************************/
        case POWER:
                if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
                runJob = new powerSpec(m_modelPointer,wind,axes->label[2],m_graph);
            break;
        case TSPLOT:
                if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
                runJob = new timeSeriesPlot(m_modelPointer,axes->label[1],m_graph,NULL,multiplot_num,multiplotPtrs);
            break;
        case M_TSPLOT:
                if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
                runJob = new m_timeSeriesPlot(ts_init_values,
                                        m_modelPointer,axes->label[1],m_graph);
            break;
        case M_TSPLOT_v:
                if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
                runJob = new m_timeSeriesPlot_v(axes->label[2],ts_init_values,
                                m_modelPointer,axes->label[1],m_graph);
            break;
        case TSPLOT_LOOP:
                if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
                runJob = new timeSeriesPlot_Loop(loop_size,m_modelPointer,axes->label[1],m_graph,
                    NULL,multiplot_num,multiplotPtrs);
            break;
        case COLOR_PLOT:
        case COLOR_PLOT_B:
                if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
                runJob = new color_plot(point_size,m_modelPointer,*axes,m_graph);
            break;
        case CONTOURLINE:
        case CONTOURLINE_B:
        case CONTOURLINE_C:
                if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
                runJob = new contourline(m_modelPointer,*axes,*xDef,*yDef,m_graph);
            break;
        case PROBABILITY:
                if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
                runJob = new probability(point_size,m_modelPointer,*axes,m_graph);
            break;
        case DISTRIBUTION:
                if( axes )
            delete axes;
            axes=conParam2xyRange(m_conBlock,3);
                runJob = new distributionJob(point_size,m_modelPointer,*axes,m_graph);
            break;
        case ACF_PLOT:
            axes->min[0]=m_conBlock.zmin;
            axes->max[0]=m_conBlock.zmax;
            runJob = new acf_plot(m_modelPointer,*axes,m_graph,lag_1,lag_2);
            break;
        case PACF_PLOT:
            axes->min[0]=m_conBlock.zmin;
            axes->max[0]=m_conBlock.zmax;
            runJob = new pacf_plot(m_modelPointer,*axes,m_graph,lag_1,lag_2);
            break;
        case MDMAP:
            runJob = new mapping(m_modelPointer,m_graph,0,1,5,3,10,"maps/default_test.map",NULL);
            break;
        case TSSET:
            if (m_conBlock.zLabel == "true") {
            runJob = new timeSeriesSet(m_modelPointer,*axes,"",true,long(m_conBlock.zmin));
            } else {
            runJob = new timeSeriesSet(m_modelPointer,*axes,"",false,long(m_conBlock.zmin));
            }
            break;
        default:
            log() << "macrodyn::doRun m_conBlock.graphTyp %i not implemented " << m_conBlock.graphTyp;
            break;
    }
//    saveFile("actualSet.par");
    qDebug() << "Switch left...\n";
    if( runJob )
    {
        log() << "starting simulation...\n";
        runJob->simulation();
        delete runJob;
    }
    else log() << "job misspecified...\n";

//	log() << "closing job...\n";

    if( axes )
    delete axes;
}

void SimLoader::setModel(const QString &model)
{
    if (model != m_modelName)
    {
        if (m_modelPointer)
        {
            delete m_modelPointer;
        }
        if ((m_modelPointer = getModel(model)))
        {
            m_modelName = model;
            emit modelChanged();
        }
        else
        {
            if ((m_modelPointer = getPubModel(model)))
            {
                m_modelName = model;
                emit modelChanged();
            }
            else
            {
                fatalError("setModel: Can't allocate m_modelPointer ", model);
            }
        }
    }
    else
    {
        qDebug() << "m_modelName = model:" << m_modelName;
        if (!m_modelPointer)
        {
            if (!(m_modelPointer = getModel(m_modelName)))
            {
                if (!(m_modelPointer = getPubModel(m_modelName)))
                {
                    fatalError("setModel: Can't allocate m_modelPointer ", m_modelName);
                }
            }
        }
    }
}

void SimLoader::setGraphTyp(int graphTyp)
{
    if (m_conBlock.graphTyp != graphTyp)
    {
        m_conBlock.graphTyp = graphTyp;
        emit graphTypChanged();
    }
}

void SimLoader::setGraphItem(QObject *object)
{
    MacrodynGraphicsItem *graph = qobject_cast<MacrodynGraphicsItem *>(object);
    if (m_graph != graph)
    {
        m_graph = graph;
        emit graphItemChanged();
    }
}
