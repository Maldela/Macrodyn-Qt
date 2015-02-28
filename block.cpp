#include "sim.h"
#include "block.h"

conParam::conParam()
{
    xLabel = "g";
    yLabel = "mtreal";
    zLabel = "w0";
    xmin = ymin = zmin = 0.0;
    xmax = 1.0;
    ymax = zmax = 2.0;
    realYmin = realZmin = 1000.0;
    realYmax = realZmax = 0.0;
    zerox = zeroy = zeroz = QColor(Qt::black);
    graphTyp = BIF2D;
}
