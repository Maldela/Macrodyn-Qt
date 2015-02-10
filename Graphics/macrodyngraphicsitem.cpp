#include "macrodyngraphicsitem.h"
#include "../logger.h"


#define LMARGIN 40
#define LOWMARGIN 20
#define RMARGIN 60
#define UPMARGIN 30
#define MAXLABELLENGTH 8
#define AXISCOLOR QColor(Qt::black)
#define AXISLABELCOLOR QColor(Qt::red)
#define XICSMARKSCOLOR QColor(Qt::darkGreen)
#define ZEROLINECOLOR QColor(Qt::yellow)


MacrodynGraphicsItem::MacrodynGraphicsItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    lmargin = LMARGIN;
    rmargin = RMARGIN;
    lowmargin = LOWMARGIN;
    upmargin = UPMARGIN;
    backgroundColor = QColor(Qt::white);

    image = new QImage();

    imageThread = new QThread();
    imageThread->start();
    m_imagePainter = new ImagePainter(this, image, &lock, &imageMutex);
    m_imagePainter->moveToThread(imageThread);


//    connect(&redrawTimer, SIGNAL(timeout()), m_imagePainter, SLOT(redraw()));

    connect(this, SIGNAL(widthChanged()), this, SLOT(handleSizeChanged()));
    connect(this, SIGNAL(heightChanged()), this, SLOT(handleSizeChanged()));

    connect(this, SIGNAL(newPoint(QPointF,QColor)), m_imagePainter, SLOT(drawPoint(QPointF,QColor)));
    connect(this, SIGNAL(newLine(QLineF,QColor)), m_imagePainter, SLOT(drawLine(QLineF,QColor)));
    connect(this, SIGNAL(newRect(QRectF,QColor)), m_imagePainter, SLOT(drawRect(QRectF,QColor)));
    connect(this, SIGNAL(newClearColumn(qreal)), m_imagePainter, SLOT(clearColumn(qreal)));
    connect(this, SIGNAL(needRedraw()), m_imagePainter, SLOT(redraw()));
    connect(m_imagePainter, SIGNAL(imageChanged()), this, SLOT(update()));
    connect(m_imagePainter, SIGNAL(imageFinished(QImage *)), this, SLOT(newImage(QImage *)));
}

MacrodynGraphicsItem::~MacrodynGraphicsItem()
{
    m_imagePainter->deleteLater();
    imageThread->quit();
    imageThread->wait();
    delete imageThread;
    delete image;
}

void MacrodynGraphicsItem::paint(QPainter *painter)
{
    imageMutex.lock();
    if (!image->isNull()) painter->drawImage(boundingRect(), *image);
    imageMutex.unlock();

    if (axis) drawAxis(painter);
    else qDebug() << "axis invalid!";
}

void MacrodynGraphicsItem::newImage(QImage *newImage)
{
    imageMutex.lock();
    delete image;
    image = newImage;
    imageMutex.unlock();
    update();
}

void MacrodynGraphicsItem::handleSizeChanged()
{
    wid = width() - lmargin - rmargin;
    hig = height() - upmargin - lowmargin;
    emit needRedraw();
}

void MacrodynGraphicsItem::setXYRange(const xyRange& range)
{
    origAxis = range;
    axis = range;
    qDebug() << "New yxRange set";
    emit needRedraw();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: drawAxis                                                  */
/* Purpose:         drawing the axis                                          */
/*                                                                            */
/* This function is responsible for drawing the axis,                         */
/* computing the marks and labeling the axis.                                 */
/* there are 3 possible color options to set:                                 */
/* AXISCOLOR sets the color of the axis                                       */
/* AXISLABELCOLOR sets the color of the string written                        */
/* XICSMARKSCOLOR sets the color of the numbers written                       */
/* MAXLABELLENGTH :Max number of chars of the label strings                   */
/*                                                                            */
/* Modified:        1994/1995 (Markus Lohmann, Jorg Nikutta)                  */
/* Last modified:   Mon Jan 31 14:59:06 MET 2000  Michael Meyer               */
/*                                                                            */
/******************************************************************************/
void MacrodynGraphicsItem::drawAxis(QPainter *painter)
{
    int i;
    int axisMark;
    QString intxLabel = axis.label.at(0).left(MAXLABELLENGTH + 2);
    QString intyLabel = axis.label.at(1).left(MAXLABELLENGTH + 2);
    QPolygon pointsX;
    pointsX << QPoint(0, 0);         /* coords for arrow on x-axis */
    pointsX << QPoint(-10, 5);
    pointsX << QPoint(-10, -5);

    QPolygon pointsY;
    pointsY << QPoint(0, 0);         /* coords for arrow on y-axis */
    pointsY << QPoint(-5, 10);
    pointsY << QPoint(5, 10);

    /* Draw Axis + ar(height()) */
    if (!painter) painter = new QPainter(image);
    painter->setPen(AXISCOLOR);
    painter->setBrush(QColor(AXISCOLOR));
    painter->drawLine(lmargin, height()-lowmargin, width()-rmargin+5, height()-lowmargin); /* X */
    pointsX.translate(width()-rmargin+10, height()-lowmargin);/*set starting point for x-arrow*/
    /*draw x-arrow */
    painter->drawPolygon(pointsX);

    painter->drawLine(lmargin, height()-lowmargin, lmargin, upmargin-10); /* Y */
    pointsY.translate(lmargin, upmargin-10);/*set starting point for y-arrow*/
    /*draw y-arrow */
    painter->drawPolygon(pointsY);

    // draw zeroline
    if ((axis.zeroline[1].isValid()) && (axis.min[1] < 0))
    {
        painter->setPen(ZEROLINECOLOR);
        int zl = upmargin+((height())-lowmargin-upmargin) * axis.max[1] / (axis.max[1]-axis.min[1]);
        painter->drawLine(lmargin, zl, width()-rmargin+5, zl);
    }

    painter->setPen(AXISLABELCOLOR);

    /* Put Label on axis */
    painter->drawText(width()-rmargin+2, height()-lowmargin-5, intxLabel);
    painter->drawText(5, upmargin-15, intyLabel);

    painter->setPen(XICSMARKSCOLOR);

    /* Draw Marks on horizontal axis */
    for (i=0; i<=4; i++)
    {
        axisMark = i*((width())-lmargin-rmargin)/4;
        QString markLabel = QString::number(axis.min[0]+i*(axis.max[0]-axis.min[0])/4);
        painter->drawLine(lmargin+axisMark, height()-lowmargin, lmargin+axisMark, height()-lowmargin+5);
        painter->drawText(lmargin+axisMark-10, height()-lowmargin+18, markLabel);
    }

    /* Draw Marks on vertical axis */
    int j,ii;
    if (axis.zeroline.at(1).isValid() && axis.min.at(1) < 0)
    {
       int zerow = (int)(((height())-lowmargin-upmargin)*axis.max[1]/(axis.max[1]-axis.min[1]));
       if ((axis.max[1]/(-axis.min[1]))>3.8) {i=4;j=1;}
       else if((axis.max[1]/(-axis.min[1]))>1.9) {i=4;j=2;}
       else if((axis.max[1]/(-axis.min[1]))>1.3) {i=3;j=2;}
       else if((axis.max[1]/(-axis.min[1]))>0.76) {i=2;j=2;}
       else if((axis.max[1]/(-axis.min[1]))>0.53) {i=2;j=3;}
       else if((axis.max[1]/(-axis.min[1]))>0.26) {i=2;j=4;}
       else {i=1;j=4;}
       for (ii=0;ii<j;ii++)
       {
           axisMark = ii*(height()-upmargin-lowmargin-zerow)/j;
           painter->drawLine(lmargin-5, height()-lowmargin-axisMark, lmargin, height()-lowmargin-axisMark);
           QString markLabel = QString::number(axis.min[1]+ii*(-axis.min[1])/j);
           painter->drawText(lmargin-17, height()-lowmargin-axisMark+5, markLabel);
       }
       for (ii=0;ii<=i;ii++)
       {
           axisMark = ii*zerow/i;
           painter->drawLine(lmargin-5, upmargin+zerow-axisMark, lmargin, upmargin+zerow-axisMark);
           QString markLabel = QString::number(ii*axis.max[1]/i);
           painter->drawText(lmargin-17, upmargin+zerow-axisMark+5, markLabel);
       }
    }
    else
    {
       for (i=0; i<=4; i++)
       {
           axisMark = i*(height()-upmargin-lowmargin)/4;
           painter->drawLine(lmargin-5, height()-lowmargin-axisMark, lmargin, height()-lowmargin-axisMark);
           QString markLabel = QString::number(axis.min[1]+i*(axis.max[1]-axis.min[1])/4);
           painter->drawText(lmargin-17, height()-lowmargin-axisMark+5, markLabel);
       }
    }
// draw color ranges for contourline plot into window
// if wished
//TODO
//    if((job==CONTOURLINE)||(job==CONTOURLINE_B)||(job==COLOR_PLOT)||(job==COLOR_PLOT_B))
//    {
//        qreal lval,rval;
//        int cqMin=base->cqMin;
//        int cqMax=base->cqMax;
//        qreal sqMin=axis.min[2]/cqMax;
//        qreal sqMax=axis.max[2]/cqMax;
//    //Print color caskets
//        int yPosCount=upmargin-1;
//        int yPosStep=(WINHEIGHT-upmargin-lowmargin)/(cqMax+17-cqMin+1+1);
//        char_chain = QString("color scale of").append(axis.label[2]);
//        XDrawString(base->displ,buffer_pix,base->ColorGC[29],
//                (width()),yPosCount,
//                char_chain,strlen(char_chain));
//        for (i=cqMin;i<=cqMax+17;i++)
//        { // paint color map
//            yPosCount+=yPosStep;
//            XFillRectangle(base->displ,buffer_pix,base->ColorGC[i],
//                (width())+50,yPosCount-yPosStep/8,15,9);
//        }
//    // print color range for interval
//        sqMax=(axis.max[2]-axis.min[2])/(cqMax+17);
//        yPosCount=upmargin+5+yPosStep;
//        i=1;
//        lval=axis.max[2]-(i*sqMax);
//        sprintf(char_chain,"[%4.3f,%4.3f]",lval,axis.max[2]);

//        XFillRectangle(base->displ,buffer_pix,base->ColorGC[29],
//            (width())+35,yPosCount-yPosStep/30,30,1);

//        XDrawString(base->displ,buffer_pix,base->ColorGC[29],
//                (width())-40,yPosCount,
//                char_chain,strlen(char_chain));
//        for(i=2;i<34;i+=2)
//        {
//            yPosCount+=yPosStep*2;
//            rval=axis.max[2]-((i+1)*sqMax);
//            sprintf(char_chain,"%4.3f",rval);

//            XFillRectangle(base->displ,buffer_pix,base->ColorGC[29],
//                (width())-10,yPosCount-yPosStep/30,75,1);

//            XDrawString(base->displ,buffer_pix,base->ColorGC[29],
//                (width())-40,yPosCount,
//                char_chain,strlen(char_chain));
//        }
//        yPosCount+=yPosStep+3;
//        i=34;
//                lval = axis.max[2]-(i*sqMax);
//        sprintf(char_chain,"%4.3f",lval);

//        XFillRectangle(base->displ,buffer_pix,base->ColorGC[29],
//            (width())-10,yPosCount-yPosStep/30,75,1);

//        XDrawString(base->displ,buffer_pix,base->ColorGC[29],
//            (width())-40,yPosCount+2,
//            char_chain,strlen(char_chain));
//    }
}

///////////////////////////////////////////////////////////////////////////////
// Class name:          graphics
// Member function:	draw_mp_names
// Purpose:		write multiple names in the window
//
// Author:		Marc Mueller May 30 1999
// Last modified:	Sun May 30 23:47:21 CEST 1999 Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void MacrodynGraphicsItem::draw_mp_names(const QStringList& names)
{
    QString string;
    for (int j=0; j<names.size(); j++)
    {
        string += names.at(j);
        string += " ";
    }
    QRect rect(QPoint(lmargin+10, upmargin-5), QPoint(width()-rmargin-10, height()-lowmargin));
    QPainter painter(image);
    //TODO
    //painter->setPen(QBrush(QColor(jnngfvtdxljn)));
    update();
}

///////////////////////////////////////////////////////////////////////////////
// Class name:          graphics
// Member function:	drawString
// Purpose:		write a string in the window
//
// Author:		Andreas Starke
// Last modified:
///////////////////////////////////////////////////////////////////////////////
void MacrodynGraphicsItem::drawString(qreal x, qreal y, const QString& text, const QColor& color, bool transform)
{
    int pixX = transform ? transformX(x) : x;
    int pixY = transform ? transformY(y) : y; // pixel coordinates
    QPainter painter(image);
    painter.setPen(color);
    painter.drawText(pixX, pixY, text);
    update();
}

///////////////////////////////////////////////////////////////////////////////
// Class name:          graphics
// Member function:	draw_color_count
// Purpose:		draw line of counted colors
//
// Author:		Marc Mueller  Jun 16 1999
// Last modified:       Wed Jun 16 16:56:38 CEST 1999 Marc Mueller
///////////////////////////////////////////////////////////////////////////////
//void MacrodynGraphicsItem::draw_color_count()
//{
//    int i;
//    int qMax=0;
//    int paint_size;
//    int cqMax=base->cqMax;
//    int cqMin=base->cqMin;
//    int size=(base->win_width)-(width())-rmargin-2;
//    int yPosStep;
//    int yPosCount;

//    if ((axis.min[2]<0)&&(axis.max[2]>0))
//    {
//        yPosStep=(WINHEIGHT-upmargin-lowmargin)/(cqMax+17-cqMin+2);
//        yPosCount=upmargin+yPosStep;
//        for(i=cqMin;i<=cqMax+17;i++) {
//            if(colorCount[i]>qMax) qMax=colorCount[i];
//        }
//        for(i=cqMin;i<=cqMax+17;i++) {
//            paint_size=2*(colorCount[i]*size)/(3*qMax);
//            if(paint_size>0)
//                XFillRectangle(base->displ,buffer_pix,base->ColorGC[i],
//                    (width())+70,yPosCount,paint_size,3);
//            yPosCount+=yPosStep;
//        }
//    }
//    if((axis.max[2]>0) && (axis.min[2]>=0)){
//        yPosStep=(WINHEIGHT-upmargin-lowmargin)/(cqMax+17-cqMin+2);
//        yPosCount=upmargin+yPosStep;
//        for(i=cqMin;i<=cqMax;i++) {
//            if(colorCount[i]>qMax) qMax=colorCount[i];
//        }
//        for(i=cqMin;i<=cqMax;i++) {
//            paint_size=2*(colorCount[i]*size)/(3*qMax);
//            if(paint_size>0)
//                XFillRectangle(base->displ,buffer_pix,base->ColorGC[i],
//                    (width())+70,yPosCount,paint_size,3);
//            yPosCount+=yPosStep;
//        }
//    }
//    if((axis.max[2]<=0) && (axis.min[2]<0)){
//        yPosStep=(WINHEIGHT-upmargin-lowmargin)/(cqMax+17-cqMin+2);
//        yPosCount=upmargin+yPosStep+120;
//        for(i=cqMin+17;i<=cqMax+17;i++) {
//            if(colorCount[i]>qMax) qMax=colorCount[i];
//        }
//        for(i=cqMin+17;i<=cqMax+17;i++) {
//            paint_size=2*(colorCount[i]*size)/(3*qMax);
//            if(paint_size>0)
//                XFillRectangle(base->displ,buffer_pix,base->ColorGC[i],
//                    (width())+70,yPosCount,paint_size,3);
//            yPosCount+=yPosStep;
//        }
//    }
//}

///////////////////////////////////////////////////////////////////////////////
//
// member function:	set_axis
// purpose:		set qMin and qMax value of axis
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////

void MacrodynGraphicsItem::set_axis(int which, qreal to_min, qreal to_max)
{
    axis.min[which] = to_min;
    axis.max[which] = to_max;
}

///////////////////////////////////////////////////////////////////////////////
//
// member function:	get_axis
// purpose:		get qMin and qMax value of axis
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////

void MacrodynGraphicsItem::get_axis(int which, qreal *to_min, qreal *to_max)
{
    *to_min = axis.min[which];
    *to_max = axis.max[which];
}

///////////////////////////////////////////////////////////////////////////////
//
// member function:	clear_window
// purpose:		flushes output window with ColorGC[0]
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////

void MacrodynGraphicsItem::clear_window()
{
    lock.lockForWrite();
    m_lines.clear();
    m_rects.clear();
    m_points.clear();
    m_clearColumns.clear();
    lock.unlock();
    emit needRedraw();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: setPoint                                                  */
/* Purpose:         draw a point on the screen                                */
/* Modified:        27.02.1995 (Markus Lohmann, Diana Gomez)                  */
/* Last modified:   Wed Jun 16 16:56:38 CEST 1999 Marc Mueller                */
/*                                                                            */
/******************************************************************************/
void MacrodynGraphicsItem::setPoint(qreal v, qreal w, const QColor& color)
{
    QPair<QPointF, QColor> pair = QPair<QPointF, QColor>(QPointF(v, w), color);
    lock.lockForWrite();
    m_points << pair;
    lock.unlock();
    emit newPoint(pair.first, pair.second);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: setBigPoint                                               */
/* Purpose:         draw a point on the screen with variable size             */
/* Modified:        mhoffmann                                                 */
/* Last modified:   Mon Sep 11 12:00:14 METDST 2000                           */
/*                                                                            */
/******************************************************************************/
void MacrodynGraphicsItem::setBigPoint(qreal v, qreal w, const QColor& color, int size)
{
    QPair<QRectF, QColor> pair = QPair<QRectF, QColor>(QRectF(v-size/2, w-size/2, size, size), color);
    lock.lockForWrite();
    m_rects << pair;
    lock.unlock();
    emit newRect(pair.first, pair.second);
}

void MacrodynGraphicsItem::setBigPoint(qreal v, qreal w, int colorInt, int size)
{
    QColor color;
    colorFromInt(color,colorInt);
    setBigPoint(v, w, color, size);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: setRect                                              */
/* Purpose:         draw a rectangle on the screen with variable size and color             */
/* Modified:        JTiwisina                			              */
/* Last modified:   Mon Jan 11 12:00:14 METDST 2015                           */
/*                                                                            */
/******************************************************************************/
void MacrodynGraphicsItem::setRect(qreal v, qreal w, qreal width, qreal height, const QColor& color)
{
    QPair<QRectF, QColor> pair = QPair<QRectF, QColor>(QRectF(v, w, width, height), color);
    lock.lockForWrite();
    m_rects << pair;
    lock.unlock();

    emit newRect(pair.first, pair.second);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: drawLine                                                  */
/* Purpose:         draw a line on the screen                                 */
/* Last modified:   18.01.2005 (Andreas Starke)                               */
/*                                                                            */
/******************************************************************************/
void MacrodynGraphicsItem::setLine(qreal x0, qreal y0, qreal x1, qreal y1, int colorInt)
{
    QColor color;
    colorFromInt(color, colorInt);

    QPair<QLineF, QColor> pair = QPair<QLineF, QColor>(QLineF(x0, y0, x1, y1), color);
    lock.lockForWrite();
    m_lines << pair;
    lock.unlock();
    emit newLine(pair.first, pair.second);
}

void MacrodynGraphicsItem::clearColumn(qreal column)
{
    lock.lockForWrite();
    m_clearColumns << column;
    lock.unlock();
    emit newClearColumn(column);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: getCoordinates                                            */
/* Purpose:         transformation of the point coordinates into pixels       */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/
int MacrodynGraphicsItem::transformX(qreal v) const
{
    //qDebug() << "v:" << v;
    int r = (v-axis.min.at(0)) * wid / (axis.max.at(0)-axis.min.at(0)) + lmargin;
    //qDebug() << "x:" << r;
    return r;
}

int MacrodynGraphicsItem::transformY(qreal w) const
{
    //qDebug() << "w:" << w;
    int r = height() - ((w-axis.min.at(1)) * hig / (axis.max.at(1)-axis.min.at(1)) + lowmargin);
    //qDebug() << "y:" << r;
    return r;
}

QPoint MacrodynGraphicsItem::transform(const QPointF &old) const
{
    return QPoint(transformX(old.x()), transformY(old.y()));
}

void MacrodynGraphicsItem::dumpGraphics(const QString& fileName) const
{
    QFile file(fileName);
    if (file.exists()) log() << "Overwriting file " << fileName;
    if (!file.open(QIODevice::WriteOnly))
    {
        log() << "Can't open file " << fileName;
        return;
    }
    QDataStream stream(&file);
    stream << image;
    log() << "Graph stored in " << fileName;
}

/*********************************
 * function to convert
 * pixel values on x-values
 *********************************/
double MacrodynGraphicsItem::pixel_to_x(int xpix) const
{
    if (axis)
        return (xpix-lmargin) * (axis.max.at(0) - axis.min.at(0)) / wid + axis.min.at(0);
    else return 0.0;
}
/*********************************
 * function to convert
 * pixel values on y-values
 *********************************/
double MacrodynGraphicsItem::pixel_to_y(int ypix) const
{
    if (axis)
        return (height()-lowmargin-ypix) * (axis.max.at(1)-axis.min.at(1)) / hig + axis.min.at(1);
    else return 0.0;
}

void MacrodynGraphicsItem::setBackgroundColor(const QColor& c)
{
    if (backgroundColor != c)
    {
        backgroundColor = c;
        emit backgroundColorChanged();
    }
}

void MacrodynGraphicsItem::colorFromInt(QColor& color, int colorInt) const
{
    switch(colorInt){
    case 0:
        color = Qt::black;
        break;
    case 1:
        color = Qt::red;
        break;
    case 2:
        color = Qt::blue;
        break;
    case 3:
        color = Qt::green;
        break;
    case 4:
        color = Qt::yellow;
        break;
    case 5:
        color = Qt::cyan;
        break;
    }
}

void MacrodynGraphicsItem::zoom(int x1, int x2, int y1, int y2)
{
//    qDebug() << "zoom:" << zoom;
    if (axis && origAxis)
    {
        qreal xmin = pixel_to_x(qMin(x1, x2));
        qreal ymin = pixel_to_y(qMax(y1, y2));
        axis.max[0] = pixel_to_x(qMax(x1, x2));
        axis.max[1] = pixel_to_y(qMin(y1, y2));
        axis.min[0] = xmin;
        axis.min[1] = ymin;
        emit needRedraw();
    }
    else qDebug() << "axis or origAxis invalid!";
}

qreal MacrodynGraphicsItem::getZoom() const
{
    qreal zoom = 1.0;
    if (axis && origAxis)
    {
        zoom = (origAxis.max.at(0) - origAxis.min.at(0)) / (axis.max.at(0) - axis.min.at(0));
    }
    return zoom;
}

ImagePainter::ImagePainter(MacrodynGraphicsItem *parent, QImage *parentImage, QReadWriteLock *listLock, QMutex *imageMutex) : QObject()
{
    m_parent = parent;
    m_parentImage = parentImage;
    m_listLock = listLock;
    m_imageMutex = imageMutex;
    m_image = NULL;
}

void ImagePainter::redraw()
{
    m_image = new QImage(m_parent->width(), m_parent->height(), QImage::Format_ARGB32_Premultiplied);
    m_image->fill(m_parent->getBackgroundColor());

    m_listLock->lockForRead();
    QPair<QPointF, QColor> pointPair;
    foreach (pointPair, m_parent->m_points)
    {
        drawPoint(pointPair.first, pointPair.second, true);
    }

    QPair<QRectF, QColor> rectPair;
    foreach (rectPair, m_parent->m_rects)
    {
        drawRect(rectPair.first, rectPair.second, true);
    }

    QPair<QLineF, QColor> linePair;
    foreach (linePair, m_parent->m_lines)
    {
        drawLine(linePair.first, linePair.second, true);
    }

    foreach (qreal column, m_parent->m_clearColumns)
    {
        clearColumn(column);
    }
    m_listLock->unlock();

    emit imageFinished(m_image);
    m_parentImage = m_image;
    m_image = NULL;
}

void ImagePainter::drawLine(const QLineF& line, const QColor& color, bool redraw)
{
    QPoint p1 = m_parent->transform(line.p1());
    QPoint p2 = m_parent->transform(line.p2());

    QPainter painter;
    if (redraw) painter.begin(m_image);
    else
    {
        m_imageMutex->lock();
        painter.begin(m_parentImage);
    }
    painter.setPen(color);
    painter.drawLine(p1, p2);

    if (!redraw)
    {
        m_imageMutex->unlock();
        emit imageChanged();
    }
}

void ImagePainter::drawPoint(const QPointF& point, const QColor& color, bool redraw)
{
    QPoint pointTransformed = m_parent->transform(point);

    QPainter painter;
    if (redraw) painter.begin(m_image);
    else
    {
        m_imageMutex->lock();
        painter.begin(m_parentImage);
    }
    painter.setPen(color);
    painter.drawPoint(pointTransformed);

    if (!redraw)
    {
        m_imageMutex->unlock();
        emit imageChanged();
    }
}

void ImagePainter::drawRect(const QRectF& rect, const QColor& color, bool redraw)
{
    QRect rectTransformed = QRect(m_parent->transform(rect.topLeft()), m_parent->transform(rect.bottomRight()));

    QPainter painter;
    if (redraw) painter.begin(m_image);
    else
    {
        m_imageMutex->lock();
        painter.begin(m_parentImage);
    }
    painter.fillRect(rectTransformed, color);

    if (!redraw)
    {
        m_imageMutex->unlock();
        emit imageChanged();
    }
}

void ImagePainter::clearColumn(qreal x, bool redraw)
{
    int col = m_parent->transformX(x);

    QPainter painter;
    if (redraw) painter.begin(m_image);
    else
    {
        m_imageMutex->lock();
        painter.begin(m_parentImage);
    }
    painter.setPen(m_parent->getBackgroundColor());
    painter.drawLine(col, 0, col, m_image->height());

    if (!redraw)
    {
        m_imageMutex->unlock();
        emit imageChanged();
    }
}
