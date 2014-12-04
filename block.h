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

struct conParam
{
    QString xLabel;                         /* X-label                        */
    QString yLabel;                         /* Y-label                        */
    QString zLabel;                         /* Z-label                        */
    qreal xmin;                              /* minimum X-value                */
    qreal xmax;                              /* maximum X-value                */
    qreal ymin;                              /* minimum Y-value                */
    qreal ymax;                              /* maximum Y-value                */
    qreal zmin;                              /* minimum Z-value                */
    qreal zmax;                              /* maximum Z-value                */
    qreal realYmin;                          /* qreal minimum Y-value           */
    qreal realYmax;                          /* qreal maximum Y-value           */
    qreal realZmin;                          /* qreal minimum Z-value           */
    qreal realZmax;                          /* qreal maximum Z-value           */
    int zerox;                              /* color of zero axis             */
    int zeroy;
    int zeroz;
    int graphTyp;                           /* 2D Bif., 3D Bif. or Attractor  */
    conParam();
};


#endif
