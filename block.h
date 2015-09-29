#ifndef BLOCK_H
#define BLOCK_H
/******************************************************************************/
/*                                                                            */
/* Module name:  block.h                                                      */
/*                                                                            */
/* Modified:      07.03.1995 (Markus Lohmann)                                 */
/* Last Modified: Wed Jun  2 15:11:56 CEST 1999 Marc Mueller                  */
/*                                                                            */
/******************************************************************************/
#include "sim.h"
#include <QColor>

struct conParam
{
    QString xLabel;                          /* X-label                        */
    QString yLabel;                          /* Y-label                        */
    QString zLabel;                          /* Z-label                        */
    QString multiPlotLabel;                  /* this parameter will be varied during the simulation   */
    qreal xmin;                              /* minimum X-value                */
    qreal xmax;                              /* maximum X-value                */
    qreal ymin;                              /* minimum Y-value                */
    qreal ymax;                              /* maximum Y-value                */
    qreal zmin;                              /* minimum Z-value                */
    qreal zmax;                              /* maximum Z-value                */
    qint64 xRes;
    qint64 yRes;
    qint64 zRes;
    qreal realYmin;                          /* qreal minimum Y-value          */
    qreal realYmax;                          /* qreal maximum Y-value          */
    qreal realZmin;                          /* qreal minimum Z-value          */
    qreal realZmax;                          /* qreal maximum Z-value          */
    QColor zerox;                            /* color of zero axis             */
    QColor zeroy;
    QColor zeroz;
    int graphTyp;                            /* 2D Bif., 3D Bif. or Attractor  */
    conParam();
};


#endif
