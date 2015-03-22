#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H

#include <QObject>
#include <QImage>
#include <QPainter>
#include <QPair>
#include <QReadWriteLock>
#include <QMutex>
#include <QSharedPointer>

#include "macrostring.h"
#include "macrodyngraphicsitem.h"
#include "../axes.h"

class MacrodynGraphicsItem;

class ImagePainter : public QObject
{
    Q_OBJECT

public:

    ImagePainter(MacrodynGraphicsItem *, QReadWriteLock *);


signals:

    void imageChanged();
    void redrawingChanged(bool);


protected slots:

    void redraw();
    void updateAxis(const xyRange&);
    void updateParentSize(const QSize&, bool = false);
    void updateSupersamplingFactor(qreal newFactor) { m_superSamplingFactor = newFactor; redraw(); }
    void updateBigPointSize(qreal size) { m_bigPointRadius = size; redraw(); }
    void updateBackgroundColor(const QColor& color) { m_backgroundColor = color; redraw(); }
    void redrawEPS();


protected:

    void drawPoint(const QPointF&, const QColor&, QPainter * = NULL);
    void drawBigPoint(const QPointF&, const QColor&, QPainter * = NULL);
    void drawRect(const QRectF&, const QColor&, QPainter * = NULL);
    void drawLine(const QLineF&, const QColor&, QPainter * = NULL);
    void drawString(const QPointF&, const QString&, const QColor&, bool, QPainter * = NULL);
    void clearColumn(qreal, QPainter * = NULL);

    QSharedPointer<QImage> m_image;
    QReadWriteLock *m_listLock;
    MacrodynGraphicsItem *m_parent;
    xyRange m_axis;
    QSize m_parentMarginedSize;
    qreal m_superSamplingFactor;
    qreal m_bigPointRadius;
    QColor m_backgroundColor;
};

#endif // IMAGEPAINTER_H
