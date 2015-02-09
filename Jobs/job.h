// File:	Job.h
// Contents:	definition of class Job
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef Job_H
#define Job_H

#include <QObject>

#include "../Models/baseModel.h"
#include "../Graphics/macrodyngraphicsitem.h"

/******************************************************************************/
/*                                                                            */
/* Class name:    Job                                                         */
/* Derived from:  -                                                           */
/* Purpose:       Virtual base class for all Jobs; contains the pure virtual  */
/*                member function simulation                                  */
/* Author:        Markus Lohmann                                              */
/* Last modified: 11.10.1994                                                  */
/*                                                                            */
/******************************************************************************/

class Job : public QObject
{
    Q_OBJECT

protected:
    qint64 length;                       // length of simulation
    qint64 limit;                        // for limit <t<= length the simulations
                                       // output is exaqMined
    baseModel* const model;            // model to analysed
    MacrodynGraphicsItem* screenGraphics;    // the output of the simulation can be
                                       // drawn on the screen ..
public:
    Job(baseModel * const, MacrodynGraphicsItem * = NULL);
    Job(baseModel * const, const qint64&, const qint64&, MacrodynGraphicsItem * = NULL);
    virtual ~Job()=0;
    virtual void setStepX(const qreal&);

public slots:
    virtual void simulation()=0;
};
#endif
//eof
