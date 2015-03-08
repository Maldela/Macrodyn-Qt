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

    void drawPoint(const QPointF&, const QColor&, bool = false, QPainter * = NULL);
    void drawBigPoint(const QPointF&, const QColor&, bool = false, QPainter * = NULL);
    void drawRect(const QRectF&, const QColor&, bool = false, QPainter * = NULL);
    void drawLine(const QLineF&, const QColor&, bool = false, QPainter * = NULL);
    void drawString(const QPointF&, const QString&, const QColor&, bool, bool = false, QPainter * = NULL);
    void clearColumn(qreal, bool = false, QPainter * = NULL);
    void redraw();
    void updateAxis(xyRange);
    void updateParentSize(QSize, bool = false);
    void updateSupersamplingFactor(int newFactor) { m_superSamplingFactor = newFactor; }


protected:

    int transformX(qreal) const;
    int transformY(qreal) const;
    QPoint transform(const QPointF&) const;
    QLine transform(const QLineF&) const;

    QSharedPointer<QImage> m_image, m_parentImage;
    QReadWriteLock *m_listLock;
    QMutex *m_imageMutex;
    MacrodynGraphicsItem *m_parent;
    xyRange m_axis;
    QSize m_parentMarginedSize;
    int m_superSamplingFactor;
    qreal m_bigPointRadius;
};

#endif // IMAGEPAINTER_H
