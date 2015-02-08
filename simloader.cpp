#include "simloader.h"
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
#include "error.h"
#include "MDMap.h"


//TODO
uint idx = 0;


int whichColorMap = 0;
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
    resList << m_conBlock.xRes << m_conBlock.yRes << m_conBlock.zRes;

    return( new xyRange(dim,labelList,qMinList,qMaxList,resList,zeroList) );
}


SimLoader::SimLoader(QObject *parent) : QObject(parent)
{
    m_modelPointer = NULL;
    m_graph = NULL;
    m_axes = NULL;
    m_runJob = NULL;
    stateSpace = NULL;
    xDef = NULL;
    yDef = NULL;
    plotLines = NULL;
}

SimLoader::~SimLoader()
{
    if (m_modelPointer) delete m_modelPointer;
    if (m_axes) delete m_axes;
    if (m_runJob) delete m_runJob;
    if (stateSpace) delete stateSpace;
    if (xDef) delete xDef;
    if (yDef) delete yDef;
    if (plotLines) delete plotLines;
}

void SimLoader::loadSimulationfromFile(const QString& fileName)
{
    lastFileName = fileName;
    QFile file(fileName);
    int divisor = 1000;

    if (!file.open(QFile::ReadOnly))
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

    stream >> m_conBlock.xmin >> m_conBlock.xmax >> m_conBlock.xRes;

    log()<<"m_conBlock.xRes = "<<m_conBlock.xRes;

    stream >> m_conBlock.yLabel;

    if (m_conBlock.yLabel == "zeroline")
    {
        m_conBlock.zeroy=1; // any positive number; different colors in graphics.C not supported yet
        stream >> m_conBlock.yLabel;
    }
    stream >> m_conBlock.ymin >> m_conBlock.ymax >> m_conBlock.yRes;

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

    qreal stepX = (m_conBlock.xmax-m_conBlock.xmin) / m_conBlock.xRes;

    if (stateSpace) delete stateSpace;
    stateSpace = NULL;
    if (xDef) delete xDef;
    xDef = NULL;
    if (yDef) delete yDef;
    yDef = NULL;

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

    m_axes = conParam2xyRange(m_conBlock, 2);
    if (m_graph) m_graph->setXYRange(*m_axes);

    qDebug() << "New m_runJob*";

    window wind = HANNING; // only case Power, leave it here

    qDebug() << "switch reached...\n";
    if (m_runJob) delete m_runJob;
    switch( m_conBlock.graphTyp )
    {
        case ATTRA:
            m_runJob = new attractor(m_modelPointer,*m_axes,m_graph);
            break;
        case D_ATTRA:
            m_runJob = new d_attractor(m_modelPointer,*m_axes,m_graph);
            break;
        case L_ATTRA:
            m_runJob = new l_attractor(m_modelPointer,*m_axes,m_graph,timeSeriesPlot::pointsize);
            break;
        case PHASE_PLOT:
            m_runJob = new phase_plot(pp_lag_size,m_modelPointer,*m_axes,m_graph,timeSeriesPlot::pointsize);
            break;
        case CELLATTRA:
            m_runJob = new cellAttractor(m_modelPointer,*m_axes,m_graph);
            break;
        case BIF2D:
            m_runJob = new bif2D(m_modelPointer,*m_axes,m_graph);
            m_runJob->setStepX(stepX);
            break;
        case BIF3D_1PAR:
            if ( m_axes )
                delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
            m_axes->res[0]=bif3D_resx;
            m_axes->res[1]=bif3D_resy;
            m_axes->res[2]=bif3D_resz;
            m_runJob = new bif3D_1par(m_modelPointer,*m_axes,m_graph,bif3D_dx,
            bif3D_dy,bif3D_dz);
            break;
        case BIF3D_2PAR:
            if( m_axes )
            delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
            m_axes->res[0]=bif3D_resx;
            m_axes->res[1]=bif3D_resy;
            m_axes->res[2]=bif3D_resz;
            m_runJob = new bif3D_2par(m_modelPointer,*m_axes,m_graph,bif3D_dx,
            bif3D_dy,bif3D_dz);
            break;
        case CYCLO3D:
            if ( m_axes )
                delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
            m_axes->res[0]=bif3D_resx;
            m_axes->res[1]=bif3D_resy;
            m_axes->res[2]=bif3D_resz;
            m_runJob = new cyclogram_3d(m_modelPointer,*m_axes,*stateSpace,m_graph,bif3D_dx,
            bif3D_dy,bif3D_dz);
            break;
        case ATTRA3D:
            if ( m_axes )
                delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
            m_axes->res[0]=bif3D_resx;
            m_axes->res[1]=bif3D_resy;
            m_axes->res[2]=bif3D_resz;
            m_runJob = new attractor_3d(m_modelPointer,*m_axes,m_graph);
            break;
        case D_BIF2D:
            m_runJob = new d_bif2D(m_modelPointer,*m_axes,m_graph);
            //m_runJob->setStepX(stepX);
            break;
        case D_BIF2D_F2:
            m_runJob = new d_bif2D_f2(m_modelPointer,*m_axes,m_graph);
            m_runJob->setStepX(stepX);
            break;
        case D_BIF2D_F4:
            m_runJob = new d_bif2D_f4(m_modelPointer,*m_axes,m_graph);
            m_runJob->setStepX(stepX);
            break;
        case DENSITY_1D:
            //m_axes=conParam2xyRange(m_conBlock,3);
            m_runJob = new density_1d(m_modelPointer,*m_axes,m_graph);
            break;
        case DENSITY_1D_1P:
                if( !m_graph )
            break;
            if( m_axes )
            delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
            m_axes->res[2]=10;
            m_runJob = new density_1d_1p(m_modelPointer,*m_axes,m_graph);
            break;
        case DENSITY_1D_VAR:
            if ( m_axes ) delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
            m_axes->res[2]=lag_1;
            m_runJob = new density_1d_var(m_modelPointer,*m_axes,m_graph);
            break;
        case INDICATOR_2D:
            if( !m_graph )
                break;
            if( m_axes )
                delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);	// what a pity
            m_runJob = new indicator_2d(m_modelPointer,*m_axes,m_graph);
            break;
        case DISCBIF2D:
            m_axes->res[0]=(short)(m_axes->max[0]-m_axes->min[0]+1);
            m_runJob = new discreteBif2D(m_modelPointer,*m_axes,m_graph);
            m_runJob->setStepX(stepX);
            break;
        case CELLBIF2D:
            m_runJob = new cellBif2D(m_modelPointer,*m_axes,m_graph);
            break;
        case PARSPACE:
            m_runJob = new parameterSpace(m_modelPointer,*m_axes,*stateSpace,m_graph);
            m_runJob->setStepX(stepX);
            break;
        case MAX_LAYP_EXP_1D:
            m_runJob = new max_lyapunov_exp_1d(m_modelPointer,*m_axes,m_graph);
            m_runJob->setStepX(stepX);
            break;
        case MAX_LAYP_EXP_2D:
            m_runJob = new max_lyapunov_exp_2d(m_modelPointer,*m_axes,m_graph);
            m_runJob->setStepX(stepX);
            break;
        case MAX_LAYP_EXP_T:
            m_runJob = new max_lyapunov_exp_t(m_modelPointer,*m_axes,m_graph);
            m_runJob->setStepX(stepX);
            break;
        case NPARMAP:
            m_runJob = new nParameterAnalysis(m_modelPointer,*m_axes,*stateSpace,
                          *xDef,*yDef,m_graph);
            m_runJob->setStepX(stepX);
            break;
        case BIFNPARAM:
            m_runJob = new bifnParam(m_modelPointer,*m_axes,*xDef,m_graph);
            m_runJob->setStepX(stepX);
            break;
        case D_BIFNPARAM:
            m_runJob = new d_bifnParam(m_modelPointer, *m_axes, *xDef, m_graph);
            m_runJob->setStepX(stepX);
            break;
        case SIMPLECELL:
            m_runJob = new simpleCellMapping(m_modelPointer,*m_axes,m_graph);
            break;
        case STATEANAL:
            m_runJob = new stateSpaceAnalysis(m_modelPointer,*m_axes,m_graph);
            break;
        case BIF2PAR:
            if( !m_graph )
                break;
            if( m_axes )
                delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
            m_axes->res[2]=10;
            m_runJob = new bif2Param(m_modelPointer,*m_axes,m_graph);
            break;
        case BASIN:
            m_runJob = new basin(m_modelPointer,*m_axes,"cycle.asc",m_graph);
            m_runJob->setStepX(stepX);
            break;
        case BASIN2:
            m_runJob = new basinTwoCycles(m_modelPointer,*m_axes,"cycle1.asc", "cycle2.asc",m_graph);
            m_runJob->setStepX(stepX);
            break;
        case HORSE: break;
        /**********************************
                graph=new graph2D(m_conBlock.xmin,m_conBlock.xmax,
                      m_conBlock.ymin,m_conBlock.ymax);
            if( graph && !m_conBlock.createPrint ) {
            xDim=graph->sizeX();
            yDim=graph->sizeY();
            }
            m_axes=new xyRange(m_conBlock.xLabel,m_conBlock.xmin,
                     m_conBlock.xmax,xDim,m_conBlock.yLabel,
                     m_conBlock.ymin,m_conBlock.ymax,yDim);
            printDevice=createPrinter(*m_axes);
            m_runJob = new horseshoe(m_modelPointer,*m_axes,m_graph);
            break;
         ************************************/
        case POWER:
            if( m_axes )
                delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
            m_runJob = new powerSpec(m_modelPointer,wind,m_axes->label[2],m_graph);
            break;
        case TSPLOT:
            if( m_axes )
                delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
            m_runJob = new timeSeriesPlot(m_modelPointer,m_axes->label[1],m_graph,NULL,multiplot_num,multiplotPtrs);
            break;
        case M_TSPLOT:
            if( m_axes )
                delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
            log()<< "ts_init_values = " << ts_init_values[1];
            m_runJob = new m_timeSeriesPlot(ts_init_values,
                                        m_modelPointer,m_axes->label[1],m_graph);
            break;
        case M_TSPLOT_v:
                if( m_axes )
            delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
                m_runJob = new m_timeSeriesPlot_v(m_axes->label[2],ts_init_values,
                                m_modelPointer,m_axes->label[1],m_graph);
            break;
        case TSPLOT_LOOP:
                if( m_axes )
            delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
                m_runJob = new timeSeriesPlot_Loop(loop_size,m_modelPointer,m_axes->label[1],m_graph,
                    NULL,multiplot_num,multiplotPtrs);
            break;
        case COLOR_PLOT:
        case COLOR_PLOT_B:
                if( m_axes )
            delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
                m_runJob = new color_plot(point_size,m_modelPointer,*m_axes,m_graph);
            break;
        case CONTOURLINE:
        case CONTOURLINE_B:
        case CONTOURLINE_C:
                if( m_axes )
            delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
                m_runJob = new contourline(m_modelPointer,*m_axes,*xDef,*yDef,m_graph);
            break;
        case PROBABILITY:
                if( m_axes )
            delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
                m_runJob = new probability(point_size,m_modelPointer,*m_axes,m_graph);
            break;
        case DISTRIBUTION:
                if( m_axes )
            delete m_axes;
            m_axes=conParam2xyRange(m_conBlock,3);
                m_runJob = new distributionJob(point_size,m_modelPointer,*m_axes,m_graph);
            break;
        case ACF_PLOT:
            m_axes->min[0]=m_conBlock.zmin;
            m_axes->max[0]=m_conBlock.zmax;
            m_runJob = new acf_plot(m_modelPointer,*m_axes,m_graph,lag_1,lag_2);
            break;
        case PACF_PLOT:
            m_axes->min[0]=m_conBlock.zmin;
            m_axes->max[0]=m_conBlock.zmax;
            m_runJob = new pacf_plot(m_modelPointer,*m_axes,m_graph,lag_1,lag_2);
            break;
        case MDMAP:
            m_runJob = new mapping(m_modelPointer,m_graph,0,1,5,3,10,"maps/default_test.map",NULL);
            break;
        case TSSET:
            if (m_conBlock.zLabel == "true") {
            m_runJob = new timeSeriesSet(m_modelPointer,*m_axes,"",true,long(m_conBlock.zmin));
            } else {
            m_runJob = new timeSeriesSet(m_modelPointer,*m_axes,"",false,long(m_conBlock.zmin));
            }
            break;
        default:
            log() << "GraphTyp not implemented:" << m_conBlock.graphTyp;
            break;
    }
//    saveFile("actualSet.par");
    qDebug() << "Switch left...\n";
}

void SimLoader::runSimulation()
{
    if( !m_runJob )
        SimLoader::loadSimulationfromFile(lastFileName);  //if there is no new Job specified, load the last .sim File again
    qDebug() << "Run simulation";

    if( m_runJob )
    {
        log() << "starting simulation...";
        m_runJob->simulation();
//        log() << "before delete m_runJob...";
//        delete m_runJob;
//        log() << "after delete m_runJob...";
        m_runJob = NULL;
    }
    else log() << "job misspecified...";

    log() << "closing job...\n";

    if( m_axes )
    delete m_axes;
    m_axes = NULL;
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
