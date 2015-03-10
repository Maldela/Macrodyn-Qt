#ifndef TRANSFORM
#define TRANSFORM

#include <QRectF>
#include <QLineF>
#include <QPointF>
#include <QSize>

#include "../axes.h"

inline int transformX(const xyRange& range, const QSize& size, qreal v)
{
    return range ? (v-range.min.at(0)) * size.width() / (range.max.at(0)-range.min.at(0)) : 0;
}

inline int transformY(const xyRange& range, const QSize& size, qreal w)
{
    return range ? size.height() - ((w-range.min.at(1)) * size.height() / (range.max.at(1)-range.min.at(1))) : 0;
}

inline QPoint transform(const xyRange& range, const QSize& size, const QPointF& old)
{
    return QPoint(transformX(range, size, old.x()), transformY(range, size, old.y()));
}

inline QLine transform(const xyRange& range, const QSize& size, const QLineF &old)
{
    return QLine(transform(range, size, old.p1()), transform(range, size, old.p2()));
}

inline QRect transform(const xyRange& range, const QSize& size, const QRectF &old)
{
    return QRect(transform(range, size, old.topLeft()), transform(range, size, old.bottomRight()));
}

#endif // TRANSFORM

