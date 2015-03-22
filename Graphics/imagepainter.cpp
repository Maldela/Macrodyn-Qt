#include <QScopedPointer>

#include "imagepainter.h"
#include "transform.h"
#include "../libboard/Board.h"
#include "../logger.h"

using namespace LibBoard;


ImagePainter::ImagePainter(MacrodynGraphicsItem *parent, QReadWriteLock *listLock) : QObject()
{
    m_parent = parent;
    m_listLock = listLock;
    m_bigPointRadius = 3;
    m_superSamplingFactor = 1;
    m_backgroundColor = QColor(Qt::white);

    connect(m_parent, SIGNAL(needRedraw()), this, SLOT(redraw()));
    connect(m_parent, SIGNAL(axisChanged(const xyRange&)), this, SLOT(updateAxis(const xyRange&)));
    connect(m_parent, SIGNAL(sizeChanged(const QSize&, bool)), this, SLOT(updateParentSize(const QSize&, bool)));
    connect(m_parent, SIGNAL(needRedrawEPS()), this, SLOT(redrawEPS()));
    connect(m_parent, SIGNAL(supersamplingChanged(qreal)), this, SLOT(updateSupersamplingFactor(qreal)));
    connect(m_parent, SIGNAL(bigPointSizeChanged(qreal)), this, SLOT(updateBigPointSize(qreal)));
    connect(m_parent, SIGNAL(backgroundColorChanged(const QColor&)), this, SLOT(updateBackgroundColor(const QColor&)));
    connect(this, SIGNAL(imageChanged()), m_parent, SLOT(update()));
    connect(this, SIGNAL(redrawingChanged(bool)), m_parent, SLOT(updateRedrawing(bool)));
}

void ImagePainter::redraw()
{
    if (QAbstractEventDispatcher::instance()->hasPendingEvents())
    {
//        qDebug() << "More pending events";
        return;
    }

    if (m_parentMarginedSize.isValid())
    {
        emit redrawingChanged(true);

        m_image = QSharedPointer<QImage>(new QImage(m_parentMarginedSize, QImage::Format_RGB32));
        m_image->fill(m_backgroundColor);
        QScopedPointer<QPainter> painter(new QPainter(m_image.data()));
        painter->setRenderHint(QPainter::TextAntialiasing);

        QReadLocker listLocker(m_listLock);
        QPair<QPointF, QColor> pointPair;
        foreach(pointPair, m_parent->m_points)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            drawPoint(pointPair.first, pointPair.second, painter.data());
        }

        foreach(pointPair, m_parent->m_bigPoints)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            drawBigPoint(pointPair.first, pointPair.second, painter.data());
        }

        QPair<QRectF, QColor> rectPair;
        foreach(rectPair, m_parent->m_rects)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            drawRect(rectPair.first, rectPair.second, painter.data());
        }

        QPair<QLineF, QColor> linePair;
        foreach(linePair, m_parent->m_lines)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            drawLine(linePair.first, linePair.second, painter.data());
        }

        foreach(qreal column, m_parent->m_clearColumns)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            clearColumn(column, painter.data());
        }

        foreach(MacroString string, m_parent->m_strings)
        {
            if (QAbstractEventDispatcher::instance()->hasPendingEvents()) return;
            drawString(string.point, string.string, string.color, string.transform, painter.data());
        }

        listLocker.unlock();
        m_parent->setImage(m_image);
        emit redrawingChanged(false);
    }
}

void ImagePainter::drawLine(const QLineF& line, const QColor& color, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        painter->begin(m_image.data());
    }

    painter->setPen(color);
    painter->drawLine(transform(m_axis, m_parentMarginedSize, line));

    if (deletePainter) delete painter;
}

void ImagePainter::drawPoint(const QPointF& point, const QColor& color, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        painter->begin(m_image.data());
    }

    painter->setPen(color);
    painter->drawPoint(transform(m_axis, m_parentMarginedSize, point));

    if (deletePainter) delete painter;
}

void ImagePainter::drawBigPoint(const QPointF& point, const QColor& color, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        painter->begin(m_image.data());
    }

    QPointF pointTransformed(transform(m_axis, m_parentMarginedSize, point));
    QPainterPath path;
    qreal size = m_bigPointRadius * m_superSamplingFactor;
    path.addEllipse(pointTransformed, size, size);
    painter->fillPath(path, color);

    if (deletePainter) delete painter;
}

void ImagePainter::drawRect(const QRectF& rect, const QColor& color, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        painter->begin(m_image.data());
    }

    QRect rectTransformed = QRect(transform(m_axis, m_parentMarginedSize, rect.topLeft()), transform(m_axis, m_parentMarginedSize, rect.bottomRight()));
    rectTransformed.setHeight(rectTransformed.height()-1);
    painter->fillRect(rectTransformed, color);

    if (deletePainter) delete painter;
}

void ImagePainter::clearColumn(qreal x, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        painter->begin(m_image.data());
    }

    int col = transformX(m_axis, m_parentMarginedSize, x);
    int height = m_image->height();
    painter->setPen(m_backgroundColor);
    painter->drawLine(col, 0, col, height);

    if (deletePainter) delete painter;
}

void ImagePainter::drawString(const QPointF& point, const QString& text, const QColor& color, bool trans, QPainter *painter)
{
    bool deletePainter = false;
    if (!painter)
    {
        painter = new QPainter;
        deletePainter = true;
        painter->begin(m_image.data());
        painter->setRenderHint(QPainter::TextAntialiasing);
    }

    QPoint pointTransformed = trans ? transform(m_axis, m_parentMarginedSize, point) : point.toPoint();
    painter->setPen(color);
    painter->drawText(pointTransformed, text);

    if (deletePainter) delete painter;
}

void ImagePainter::updateAxis(const xyRange& newAxis)
{
    m_axis = newAxis;
    redraw();
}

void ImagePainter::updateParentSize(const QSize& newSize, bool simulating)
{
    m_parentMarginedSize = newSize * m_superSamplingFactor;
    if (!simulating) redraw();
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
