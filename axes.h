#ifndef AXES_H
#define AXES_H


/******************************************************************************/
/*                                                                            */
/* Module name:  axes.h                                                       */
/* Contents:     Class definition of the class xyRange                        */
/*                                                                            */
/* Last Modified: 22.02.1995 (Markus Lohmann)                                 */
/*                                                                            */
/******************************************************************************/

#include "sim.h"
#include <QColor>


/******************************************************************************/
/*                                                                            */
/* Class name:    xyRange                                                     */
/* Derived from:  -                                                           */
/* Purpose:       used to define a part of the state space or the parameter   */
/*                space under consideration                                   */
/*                                                                            */
/* Modified:      07.03.1995 (Markus Lohmann)                                 */
/* Last Modified: Wed Jun  2 16:03:59 CEST 1999 Marc Mueller                  */
/*                                                                            */
/******************************************************************************/

class xyRange
{

friend QTextStream& operator<<(QTextStream&, const xyRange&);

public:

    int dimension;		// dimension of the state space or parameter
				// space under consideration
    QList<QString> label;		// array for the labels
    QList<qreal> min;			// array for the minimum values (for each
				// dimension)
    QList<qreal> max;			// dto. for the maximum values
    QList<qint64> res;			// resolution used in each dimension
    QList<QColor> zeroline;		// -1 non; > color of zeroline

    xyRange() {}
    xyRange(int, const QList<QString>&, const QList<qreal>&, const QList<qreal>&, const QList<qint64>&, const QList<QColor>&);	// constructor
    xyRange(int, const QList<QString>&, const QList<qreal>&, const QList<qreal>&, const QList<qint64>&);	// old constructor
    xyRange(QTextStream&);         // create instance from a file
    xyRange(const xyRange&);	// copy constructor
    ~xyRange() {}			// destructor
    bool inRange(const QList<qreal *>&) const; // determines wether the argument
				// lies in the state space or parameter space
				// under consideration or not
};   

#endif
