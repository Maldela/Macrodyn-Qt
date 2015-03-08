#include <QScopedPointer>

#include "imagepainter.h"
#include "../libboard/Board.h"
#include "../logger.h"

using namespace LibBoard;


ImagePainter::ImagePainter(MacrodynGraphicsItem *parent, const QSharedPointer<QImage>& parentImage, QReadWriteLock *listLock, QMutex *imageMutex) : QObject()
{
    m_parent = parent;
    m_parentImage = parentImage;
    m_listLock = listLock;
    m_imageMutex = imageMutex;
    m_bigPointRadius = 3;
    m_superSamplingFactor = 2;
}

void ImagePainter::redraw()
{
    if (QAbstractEventDispatcher::instance()->hasPendingEvents())
    {
//        qDebug() << "More pending events";
        return;
    }

    emit startRedraw();

    if (m_parentMarginedSize.isValid())
    {
        m_image = QSharedPointer<QImage>(new QImage(m_parentMarginedSize, QImage::Format_RGB32));
        m_image->fill(m_parent->getBackgroundColor());
        QScopedPointer<QPainter> painter(new QPainter(m_image.data()));
        painter->setRenderHint(QPainter::TextAntialiasing);

        QReadLocker listLocker(m_listLock);
        QPair<QPointF, QColor> pointPair;
        foreach(pointPair, m_parent->m_points)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            drawPoint(pointPair.first, pointPair.second, true, painter.data());
        }

        foreach(pointPair, m_parent->m_bigPoints)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            drawBigPoint(pointPair.first, pointPair.second, true, painter.data());
        }

        QPair<QRectF, QColor> rectPair;
        foreach(rectPair, m_parent->m_rects)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            drawRect(rectPair.first, rectPair.second, true, painter.data());
        }

        QPair<QLineF, QColor> linePair;
        foreach(linePair, m_parent->m_lines)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            drawLine(linePair.first, linePair.second, true, painter.data());
        }

        foreach(qreal column, m_parent->m_clearColumns)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            clearColumn(column, painter.data());
        }

        foreach(MacroString string, m_parent->m_strings)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            drawString(string.point, string.string, string.color, string.transform, true, painter.data());
        }

        emit imageFinished(m_image);
        m_parentImage = m_image;
    }
}

void ImagePainter::drawLine(const QLineF& line, const QColor& color, bool redraw, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        if (redraw) painter->begin(m_image.data());
        else
        {
            m_imageMutex->lock();
            painter->begin(m_parentImage.data());
        }
    }

    painter->setPen(color);
    painter->drawLine(transform(line));

    if (!redraw)
    {
        m_imageMutex->unlock();
        emit imageFinished(m_parentImage);
    }
    if (deletePainter) delete painter;
}

void ImagePainter::drawPoint(const QPointF& point, const QColor& color, bool redraw, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        if (redraw) painter->begin(m_image.data());
        else
        {
            m_imageMutex->lock();
            painter->begin(m_parentImage.data());
        }
    }

    painter->setPen(color);
    painter->drawPoint(transform(point));

    if (!redraw)
    {
        m_imageMutex->unlock();
        emit imageFinished(m_parentImage);
    }
    if (deletePainter) delete painter;
}

void ImagePainter::drawBigPoint(const QPointF& point, const QColor& color, bool redraw, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        if (redraw) painter->begin(m_image.data());
        else
        {
            m_imageMutex->lock();
            painter->begin(m_parentImage.data());
        }
    }

    QPointF pointTransformed(transform(point));
    QPainterPath path;
    path.addEllipse(pointTransformed, m_bigPointRadius, m_bigPointRadius);
    painter->fillPath(path, color);

    if (!redraw)
    {
        m_imageMutex->unlock();
        emit imageFinished(m_parentImage);
    }
    if (deletePainter) delete painter;
}

void ImagePainter::drawRect(const QRectF& rect, const QColor& color, bool redraw, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        if (redraw) painter->begin(m_image.data());
        else
        {
            m_imageMutex->lock();
            painter->begin(m_parentImage.data());
        }
    }

    QRect rectTransformed = QRect(transform(rect.topLeft()), transform(rect.bottomRight()));
    rectTransformed.setHeight(rectTransformed.height()-1);
    painter->fillRect(rectTransformed, color);

    if (!redraw)
    {
        m_imageMutex->unlock();
        emit imageFinished(m_parentImage);
    }
    if (deletePainter) delete painter;
}

void ImagePainter::clearColumn(qreal x, bool redraw, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        if (redraw) painter->begin(m_image.data());
        else
        {
            m_imageMutex->lock();
            painter->begin(m_parentImage.data());
        }
    }

    int col = transformX(x);
    int height = redraw ? m_image->height() : m_parentImage->height();
    painter->setPen(m_parent->getBackgroundColor());
    painter->drawLine(col, 0, col, height);

    if (!redraw)
    {
        m_imageMutex->unlock();
        emit imageFinished(m_parentImage);
    }
    if (deletePainter) delete painter;
}

void ImagePainter::drawString(const QPointF& point, const QString& text, const QColor& color, bool trans, bool redraw, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        if (redraw) painter->begin(m_image.data());
        else
        {
            m_imageMutex->lock();
            painter->begin(m_parentImage.data());
        }
        painter->setRenderHint(QPainter::TextAntialiasing);
    }

    QPoint pointTransformed = trans ? transform(point) : point.toPoint();
    painter->setPen(color);
    painter->drawText(pointTransformed, text);

    if (!redraw)
    {
        m_imageMutex->unlock();
        emit imageFinished(m_parentImage);
    }
    if (deletePainter) delete painter;
}

void ImagePainter::updateAxis(xyRange newAxis)
{
    int x = transformX(newAxis.min.at(0));
    int y = transformY(newAxis.max.at(1));
    int width = transformX(newAxis.max.at(0)) - x;
    int height = transformY(newAxis.min.at(1)) - y;
    m_imageMutex->lock();
    if (!m_parentImage->isNull())
    {
        *m_parentImage = m_parentImage->copy(x, y, width, height).scaled(m_parentImage->size());
        emit imageChanged();
    }
    m_imageMutex->unlock();
    m_axis = newAxis;
    redraw();
}

void ImagePainter::updateParentSize(QSize newSize, bool simulating)
{
    m_parentMarginedSize = newSize * m_superSamplingFactor;
    if (simulating)
    {
        QMutexLocker lock(m_imageMutex);
        if (!m_parentImage->isNull())
        {
            *m_parentImage = m_parentImage->scaled(m_parentMarginedSize);
            emit imageChanged();
        }
    }
    else redraw();
}

int ImagePainter::transformX(qreal v) const
{
    return m_axis ? (v-m_axis.min.at(0)) * m_parentMarginedSize.width() / (m_axis.max.at(0)-m_axis.min.at(0)) : 0;
}

int ImagePainter::transformY(qreal w) const
{
    return m_axis ? m_parentMarginedSize.height() - ((w-m_axis.min.at(1)) * m_parentMarginedSize.height() / (m_axis.max.at(1)-m_axis.min.at(1))) : 0;
}

QPoint ImagePainter::transform(const QPointF& old) const
{
    return QPoint (transformX(old.x()), transformY(old.y()));
}

QLine ImagePainter::transform(const QLineF &old) const
{
    return QLine (transform(old.p1()), transform(old.p2()));
}

void ImagePainter::redrawEPS()
{
//    log() << "redrawEPS()";
//    m_image = QSharedPointer<QImage>(new QImage(m_parentMarginedSize, QImage::Format_RGB32));
//    m_image->fill(m_parent->getBackgroundColor());

//    m_listLock->lockForRead();

//    Board board;
//    board << Text( -0.45, -0.2, "MacroDy",
//                  Fonts::Helvetica, "'Bookman Old Style',Verdana", 64.0f, Color::Green );
//    board.setLineWidth( 1 ).setPenColorRGBi( 255, 100, 0 );
//    board.setLineStyle( Shape::SolidStyle );
//    board.setLineJoin( Shape::MiterJoin );
//    board.setLineCap( Shape::RoundCap );
//    board.drawLine( -0.5, -0.27, 0.5, -0.27 );
//    board.addDuplicates( board.last(), 10, 0, -0.02 );

//    Point p = board.last<Line>().boundingBox().topLeft();
//    int n = 20;
//    double angle = -M_PI/(2*n);
//    while ( n-- )
//        board << board.last<Line>().rotated( angle, p );

//    n = 30;
//    angle = -M_PI/(n);
//    while ( n-- )
//        board << board.last<Line>().scaled( 0.95 ).rotated( angle, p );


//    QPair<QPointF, QColor> pointPair;
//    foreach(pointPair, m_parent->m_points)
//    {
//        QPoint pointTransformed = m_parent->transform(pointPair.first);
//        board.setPenColorRGBi( pointPair.second.red(), pointPair.second.green(), pointPair.second.blue(), pointPair.second.alpha() );
//        board.drawDot(pointTransformed.x(),pointTransformed.y());
//        //drawPoint(pointPair.first, pointPair.second, true);
//    }

//    QPair<QRectF, QColor> rectPair;

//    foreach(rectPair, m_parent->m_rects)
//    {
//        QRect rectTransformed = QRect(m_parent->transform(rectPair.first.topLeft()), m_parent->transform(rectPair.first.bottomRight()));
//        rectTransformed.setHeight(rectTransformed.height()-1);
//        //board.setPenColorRGBi( rectPair.second.red(), rectPair.second.green(), rectPair.second.blue(), rectPair.second.alpha() );
//        //board.drawDot(rectTransformed.x(),rectTransformed.y());
//        board.fillRectangle(rectTransformed.x(),rectTransformed.y(),rectTransformed.width(),rectTransformed.height());
//        //drawRect(rectPair.first, rectPair.second, true);
//    }

//    QPair<QLineF, QColor> linePair;
//    foreach(linePair, m_parent->m_lines)
//    {
//        QPoint p1 = m_parent->transform(linePair.first.p1());
//        QPoint p2 = m_parent->transform(linePair.first.p2());
//        board.setLineWidth( 1 ).setPenColorRGBi( linePair.second.red(), linePair.second.green(), linePair.second.blue(), linePair.second.alpha() );
//        board.drawLine( p1.x(), p1.y(), p2.x(), p2.y());
//        //drawLine(linePair.first, linePair.second, true);
//    }

//    foreach(qreal column, m_parent->m_clearColumns)
//    {
//        clearColumn(column);
//    }

//    foreach(MacroString string, m_parent->m_strings)
//    {
//        drawString(string.point, string.string, string.color, string.transform, true);
//    }
//    m_listLock->unlock();

//    board.saveEPS( "/Users/jtiwi/Documents/build-Macrodyn-Qt-Desktop_Qt_5_3_clang_64bit-Debug/testBild7.eps", Board::A4 );
//    emit imageFinished(m_image);
//    m_parentImage = m_image;
}
