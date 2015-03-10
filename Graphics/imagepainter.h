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

    ImagePainter(MacrodynGraphicsItem *, const QSharedPointer<QImage>&, QReadWriteLock *, QMutex *);


signals:

    void imageChanged();
    void imageFinished(QSharedPointer<QImage>);
    void startRedraw();


protected slots:

    void redraw();
    void updateAxis(const xyRange&);
    void updateParentSize(const QSize&, bool = false);
    void updateSupersamplingFactor(qreal newFactor) { m_superSamplingFactor = newFactor; redraw(); }
    void updateBigPointSize(qreal size) { m_bigPointRadius = size; redraw(); }
    void redrawEPS();


protected:

    void drawPoint(const QPointF&, const QColor&, QPainter * = NULL);
    void drawBigPoint(const QPointF&, const QColor&, QPainter * = NULL);
    void drawRect(const QRectF&, const QColor&, QPainter * = NULL);
    void drawLine(const QLineF&, const QColor&, QPainter * = NULL);
    void drawString(const QPointF&, const QString&, const QColor&, bool, QPainter * = NULL);
    void clearColumn(qreal, QPainter * = NULL);

    QSharedPointer<QImage> m_image, m_parentImage;
    QReadWriteLock *m_listLock;
    QMutex *m_imageMutex;
    MacrodynGraphicsItem *m_parent;
    xyRange m_axis;
    QSize m_parentMarginedSize;
    qreal m_superSamplingFactor;
    qreal m_bigPointRadius;
};

#endif // IMAGEPAINTER_H
