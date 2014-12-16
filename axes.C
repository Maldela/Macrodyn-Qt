/******************************************************************************/
/*                                                                            */
/* Module name:  axes.C                                                       */
/* Contents:     Member functions of the class xyRange                        */
/*                                                                            */
/* Modified:     22.02.1995 (Markus Lohmann)                                  */
/* Last modified:Wed Jun  2 16:03:59 CEST 1999 (Marc Mueller)                 */
/*                                                                            */
/******************************************************************************/
#include "axes.h"
#include "logger.h"


/******************************************************************************/
/*                                                                            */
/* Class name:      xyRange                                                   */
/* Member function: xyRange                                                   */
/* Purpose:         constructor                                               */
/* Last modified:   22.02.1995 (Markus Lohmann)                               */
/* Last modified:   Wed Jun  2 16:03:59 CEST 1999  (Marc Mueller)             */
/*                                                                            */
/******************************************************************************/
xyRange::xyRange(int dim, const QList<QString>& names, const QList<qreal>& mins,
         const QList<qreal>& maxs, const QList<qint64>& count, const QList<QColor>& zerol)
{
    dimension = dim;
    label = names;
    while (label.size() < dimension-1) label.append(QString("Unlabeled"));
    min = mins;
    while (min.size() < dimension-1) min.append(min.last());
    max = maxs;
    while (max.size() < dimension-1) max.append(max.last());
    res = count;
    while (res.size() < dimension-1) res.append(res.last());
    zeroline = zerol;
    while (zeroline.size() < dimension-1) zeroline.append(QColor(Qt::transparent));
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xyRange                                                   */
/* Member function: xyRange                                                   */
/* Purpose:         constructor                                               */
/* Last modified:   22.02.1995 (Markus Lohmann)                               */
/* Last modified:   Wed Jun  2 16:03:59 CEST 1999  (Marc Mueller)             */
/*                                                                            */
/******************************************************************************/
xyRange::xyRange(int dim, const QList<QString>& names, const QList<qreal>& mins,
         const QList<qreal>& maxs, const QList<qint64>& count)
{
    dimension = dim;
    label = names;
    while (label.size() < dimension-1) label.append(QString("Unlabeled"));
    min = mins;
    while (min.size() < dimension-1) min.append(min.last());
    max = maxs;
    while (max.size() < dimension-1) max.append(max.last());
    res = count;
    while (res.size() < dimension-1) res.append(res.last());
    while (zeroline.size() < dimension-1) zeroline.append(QColor(Qt::transparent));
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xyRange                                                   */
/* Member function: xyRange                                                   */
/* Purpose:         constructor that reads from a file                        */
/* Last modified:   22.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/
xyRange::xyRange(QTextStream& inFile)
{
    inFile >> dimension;
    log() << "Dimension:" << dimension;
    for (int i=0; i<dimension; i++)
    {
        QString string;
        inFile >> string;
        label << string;
        qreal num;
        inFile >> num;
        min << num;
        inFile >> num;
        max << num;
        inFile >> num;
        res << num;
    }
}

void xyRange::toLog()
{
    log() << "Axes.C::toLog():";
    log() << "Dimension:" << dimension;
    for (int i=0; i<dimension; i++)
    {
        log() << "axis:" << i;
        log() << "label:" << label[i];
        log() << "min:" << min[i];
        log() << "max:" << max[i];
        log() << "res:" << res[i];
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xyRange                                                   */
/* Member function: xyRange                                                   */
/* Purpose:         copy constructor                                          */
/* Last modified:   22.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/
xyRange::xyRange(const xyRange& src)
{
    dimension = src.dimension;
    label = src.label;
    min = src.min;
    max = src.max;
    res = src.res;
    zeroline = src.zeroline;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      xyRange                                                   */
/* Member function: inRange                                                   */
/* Purpose:         checks wether the arguments is in range or not            */
/* Last modified:   22.02.1995 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/
bool xyRange::inRange(const QList<qreal *>& toCheck) const
{
    bool result = true;
    int i = 0;

    if (toCheck.size() < dimension - 1)
    {
        log() << "xyRange::inRange: QList<qreal *> toCheck does not contain enough elements!";
    }
    while (result && i < dimension)
    {
        result = (*toCheck.at(i) >= min[i]) && (*toCheck.at(i) <= max[i]);
        i++;
    }
    return result;
}

/******************************************************************************/
/*                                                                            */
/* Function:        operator<<                                                */
/* Purpose:         overloading of the operator << for use with an instance   */
/*                  of xyRange                                                */
/* Modified:        22.02.1995 (Markus Lohmann)                               */
/* Last modified:   Wed Jun  2 16:03:59 CEST 1999  (Marc Mueller)             */
/*                                                                            */
/******************************************************************************/
QTextStream& operator<<(QTextStream& os,const xyRange& axes)
{
    os << "Dimension: " << axes.dimension;
    os << "Rest: ";
    for(int i=0;i<axes.dimension;i++)
    {
        os << "label[" << i << "]=" << axes.label[i];
        os << "min[" << i << "]=" << axes.min[i];
        os << "max[" << i << "]=" << axes.max[i];
        os << "res[" << i << "]=" << axes.res[i];
    //	os << "zl[" << i << "]=" << axes.zeroline[i];
    }
    return os;
}
