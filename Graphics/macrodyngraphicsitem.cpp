#include "macrodyngraphicsitem.h"
#include "transform.h"
#include "../logger.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QtWidgets>

#define LMARGIN 40
#define LOWMARGIN 30
#define RMARGIN 30
#define UPMARGIN 30
#define MAXLABELLENGTH 8
#define AXISCOLOR QColor(Qt::black)
#define AXISLABELCOLOR QColor(Qt::red)
#define XICSMARKSCOLOR QColor(Qt::black)
#define ZEROLINECOLOR QColor(Qt::yellow)


MacrodynGraphicsItem::MacrodynGraphicsItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    lmargin = LMARGIN;
    rmargin = RMARGIN;
    lowmargin = LOWMARGIN;
    upmargin = UPMARGIN;
    m_supersampling = 1;
    m_backgroundColor = QColor(Qt::white);
    m_image = QSharedPointer<QImage>(new QImage);

    m_imageThread.start();
    m_imagePainter = new ImagePainter(this, &m_listLock);
    m_imagePainter->moveToThread(&m_imageThread);

    connect(this, SIGNAL(widthChanged()), this, SLOT(handleSizeChanged()));
    connect(this, SIGNAL(heightChanged()), this, SLOT(handleSizeChanged()));
}

MacrodynGraphicsItem::~MacrodynGraphicsItem()
{
    m_imagePainter->deleteLater();
    m_imageThread.quit();
    m_imageThread.wait();
}

void MacrodynGraphicsItem::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    m_imageMutex.lock();
    if (!m_image.isNull() && !m_image->isNull())
        painter->drawImage(QRect(lmargin, upmargin, wid, hig), *m_image);
    m_imageMutex.unlock();

    drawAxis(painter);
//    else qDebug() << "axis invalid!";
}

void MacrodynGraphicsItem::newImage(QSharedPointer<QImage> newImage)
{
    m_imageMutex.lock();
    m_image = newImage;
    m_imageMutex.unlock();

    m_redrawing = false;
    emit redrawingChanged();

    update();
}

void MacrodynGraphicsItem::handleSizeChanged()
{
    wid = width() - lmargin - rmargin;
    hig = height() - upmargin - lowmargin;

    emit sizeChanged(QSize(wid, hig), m_simulating);
}

void MacrodynGraphicsItem::setXYRange(const xyRange& newAxis)
{
    qDebug() << "setyxRange";
    if (newAxis/* && newAxis != m_origAxis*/)
    {
        m_origAxis = newAxis;
        m_axis = newAxis;
        qDebug() << "New yxRange set";
        emit axisChanged(m_axis);
    }
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
    QString intxLabel = m_axis ? m_axis.label.at(0).left(MAXLABELLENGTH + 2) : "";
    QString intyLabel = m_axis ? m_axis.label.at(1).left(MAXLABELLENGTH + 2) : "";
    QPolygon pointsX;
    pointsX << QPoint(0, 0);         /* coords for arrow on x-axis */
    pointsX << QPoint(-10, 5);
    pointsX << QPoint(-10, -5);

    QPolygon pointsY;
    pointsY << QPoint(0, 0);         /* coords for arrow on y-axis */
    pointsY << QPoint(-5, 10);
    pointsY << QPoint(5, 10);

    /* Draw Axis + ar(height()) */
    if (!painter) painter = new QPainter(m_image.data());
    painter->setPen(AXISCOLOR);
    painter->setBrush(AXISCOLOR);
    painter->drawLine(lmargin, height()-lowmargin, width()-rmargin+5, height()-lowmargin); /* X */
    pointsX.translate(width()-rmargin+10, height()-lowmargin);/*set starting point for x-arrow*/
    /*draw x-arrow */
    painter->drawPolygon(pointsX);

    painter->drawLine(lmargin, height()-lowmargin, lmargin, upmargin-10); /* Y */
    pointsY.translate(lmargin, upmargin-10);/*set starting point for y-arrow*/
    /*draw y-arrow */
    painter->drawPolygon(pointsY);

//    // draw zeroline
//    if ((m_axis && m_axis.zeroline[1].isValid()) && (m_axis.min[1] < 0))
//    {
//        painter->setPen(ZEROLINECOLOR);
//        int zl = upmargin+((height())-lowmargin-upmargin) * m_axis.max[1] / (m_axis.max[1]-m_axis.min[1]);
//        painter->drawLine(lmargin, zl, width()-rmargin+5, zl);
//    }

    painter->setPen(AXISLABELCOLOR);

    /* Put Label on axis */
    painter->drawText(width()-rmargin+2, height()-lowmargin-5, intxLabel);
    painter->drawText(5, upmargin-15, intyLabel);

    painter->setPen(XICSMARKSCOLOR);

    /* Draw Marks on horizontal axis */
    for (i=0; i<=4; i++)
    {
        axisMark = i*((width())-lmargin-rmargin)/4;
        QString markLabel = m_axis ? QString::number(m_axis.min[0]+i*(m_axis.max[0]-m_axis.min[0])/4) : "";
        painter->drawLine(lmargin+axisMark, height()-lowmargin, lmargin+axisMark, height()-lowmargin+5);
        painter->drawText(lmargin+axisMark-10, height()-lowmargin+18, markLabel);
    }

    /* Draw Marks on vertical axis */
    int j,ii;
    if (m_axis && m_axis.zeroline.at(1).isValid() && m_axis.min.at(1) < 0)
    {
       int zerow = (int)(((height())-lowmargin-upmargin)*m_axis.max[1]/(m_axis.max[1]-m_axis.min[1]));
       if ((m_axis.max[1]/(-m_axis.min[1]))>3.8) {i=4;j=1;}
       else if((m_axis.max[1]/(-m_axis.min[1]))>1.9) {i=4;j=2;}
       else if((m_axis.max[1]/(-m_axis.min[1]))>1.3) {i=3;j=2;}
       else if((m_axis.max[1]/(-m_axis.min[1]))>0.76) {i=2;j=2;}
       else if((m_axis.max[1]/(-m_axis.min[1]))>0.53) {i=2;j=3;}
       else if((m_axis.max[1]/(-m_axis.min[1]))>0.26) {i=2;j=4;}
       else {i=1;j=4;}
       for (ii=0;ii<j;ii++)
       {
           axisMark = ii*(height()-upmargin-lowmargin-zerow)/j;
           painter->drawLine(lmargin-5, height()-lowmargin-axisMark, lmargin, height()-lowmargin-axisMark);
           QString markLabel = QString::number(m_axis.min[1]+ii*(-m_axis.min[1])/j);
           painter->drawText(lmargin-40, height()-lowmargin-axisMark+5, markLabel);
       }
       for (ii=0;ii<=i;ii++)
       {
           axisMark = ii*zerow/i;
           painter->drawLine(lmargin-5, upmargin+zerow-axisMark, lmargin, upmargin+zerow-axisMark);
           QString markLabel = QString::number(ii*m_axis.max[1]/i);
           painter->drawText(lmargin-40, upmargin+zerow-axisMark+5, markLabel);
       }
    }
    else
    {
       for (i=0; i<=4; i++)
       {
           axisMark = i*(height()-upmargin-lowmargin)/4;
           painter->drawLine(lmargin-5, height()-lowmargin-axisMark, lmargin, height()-lowmargin-axisMark);
           QString markLabel = m_axis ? QString::number(m_axis.min[1]+i*(m_axis.max[1]-m_axis.min[1])/4) : "";
           painter->drawText(lmargin-40, height()-lowmargin-axisMark+5, markLabel);
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
    QPainter painter(m_image.data());
    //TODO
    //painter->setPen(QBrush(QColor(jnngfvtdxljn)));
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
    m_axis.min[which] = to_min;
    m_axis.max[which] = to_max;
    emit axisChanged(m_axis);
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

void MacrodynGraphicsItem::get_axis(int which, qreal *to_min, qreal *to_max) const
{
    *to_min = m_axis.min.at(which);
    *to_max = m_axis.max.at(which);
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
    m_listLock.lockForWrite();
    m_lines.clear();
    m_rects.clear();
    m_points.clear();
    m_bigPoints.clear();
    m_clearColumns.clear();
    m_strings.clear();
    m_listLock.unlock();

    m_imageMutex.lock();
    m_image->fill(m_backgroundColor);
    m_imageMutex.unlock();

    update();
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
    QPair<QPointF, QColor> pair(QPointF(v, w), color);

    QScopedPointer<QPainter>painter(new QPainter);
    m_imageMutex.lock();
    if (!m_image->isNull())
    {
        painter->begin(m_image.data());
        painter->setPen(color);
        painter->drawPoint(::transform(m_axis, m_image->size(), pair.first));
        painter->end();
    }
    m_imageMutex.unlock();

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);

    m_listLock.lockForWrite();
    m_points << pair;
    m_listLock.unlock();
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
    QPair<QPointF, QColor> pair(QPointF(v, w), color);

    QScopedPointer<QPainter>painter(new QPainter);
    QPainterPath path;

    m_imageMutex.lock();
    if (!m_image->isNull())
    {
        painter->begin(m_image.data());
        painter->setPen(color);
        qreal size = m_bigPointSize * m_supersampling;
        path.addEllipse(::transform(m_axis, m_image->size(), pair.first), size, size);
        painter->fillPath(path, color);
        painter->end();
    }
    m_imageMutex.unlock();

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);

    m_listLock.lockForWrite();
    m_bigPoints << pair;
    m_listLock.unlock();
}

void MacrodynGraphicsItem::setBigPoint(qreal v, qreal w, int colorInt, int size)
{
    QColor color;
    colorFromInt(color, colorInt);
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
    QPair<QRectF, QColor> pair(QRectF(v, w, width, height), color);

    QScopedPointer<QPainter>painter(new QPainter);
    m_imageMutex.lock();
    if (!m_image->isNull())
    {
        painter->begin(m_image.data());
        painter->setPen(color);
        painter->drawRect(::transform(m_axis, m_image->size(), pair.first));
        painter->end();
    }
    m_imageMutex.unlock();

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);

    m_listLock.lockForWrite();
    m_rects << pair;
    m_listLock.unlock();
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
    QPair<QLineF, QColor> pair(QLineF(x0, y0, x1, y1), color);

    QScopedPointer<QPainter>painter(new QPainter);
    m_imageMutex.lock();
    if (!m_image->isNull())
    {
        painter->begin(m_image.data());
        painter->setPen(color);
        painter->drawLine(::transform(m_axis, m_image->size(), pair.first));
        painter->end();
    }
    m_imageMutex.unlock();

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);

    m_listLock.lockForWrite();
    m_lines << pair;
    m_listLock.unlock();
}

void MacrodynGraphicsItem::setString(qreal x, qreal y, const QString& s, const QColor& color, bool trans)
{
    QPoint pointTransformed = trans ? ::transform(m_axis, m_image->size(), QPointF(x, y)) : QPoint(x, y);

    QScopedPointer<QPainter>painter(new QPainter);
    m_imageMutex.lock();
    if (!m_image->isNull())
    {
        painter->begin(m_image.data());
        painter->setRenderHint(QPainter::TextAntialiasing);
        painter->setPen(color);
        painter->drawText(pointTransformed, s);
        painter->end();
    }
    m_imageMutex.unlock();

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);

    MacroString string(s, QPointF(x, y), color, trans);
    m_listLock.lockForWrite();
    m_strings << string;
    m_listLock.unlock();
}

void MacrodynGraphicsItem::clearColumn(qreal column)
{
    QScopedPointer<QPainter>painter(new QPainter);
    m_imageMutex.lock();
    if (!m_image->isNull())
    {
        painter->begin(m_image.data());
        painter->setPen(m_backgroundColor);
        painter->drawLine(transformX(m_axis, m_image->size(), column), 0, transformX(m_axis, m_image->size(), column), m_image->height());
        painter->end();
    }
    m_imageMutex.unlock();

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);

    m_listLock.lockForWrite();
    m_clearColumns << column;
    m_listLock.unlock();
}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: getCoordinates                                            */
/* Purpose:         transformation of the point coordinates into pixels       */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/


/*********************************
 * function to convert
 * pixel values on x-values
 *********************************/
double MacrodynGraphicsItem::pixel_to_x(int xpix) const
{
    double r = 0.0;
    if (m_axis)
        r = (xpix-lmargin) * (m_axis.max.at(0) - m_axis.min.at(0)) / wid + m_axis.min.at(0);
    return r;
}
/*********************************
 * function to convert
 * pixel values on y-values
 *********************************/
double MacrodynGraphicsItem::pixel_to_y(int ypix) const
{
    double r = 0.0;
    if (m_axis)
        r = (height()-lowmargin-ypix) * (m_axis.max.at(1)-m_axis.min.at(1)) / hig + m_axis.min.at(1);
    return r;
}

void MacrodynGraphicsItem::setBackgroundColor(const QColor& c)
{
    if (m_backgroundColor != c)
    {
        m_backgroundColor = c;
        emit backgroundColorChanged(c);
    }
}

void MacrodynGraphicsItem::colorFromInt(QColor& color, int colorInt) const
{
    switch(colorInt){
    case 0:
        color = QColor(255,255,0,255);
        break;
    case 1:
        color = QColor(0,255,255,255);
        break;
    case 2:
        color = QColor(0,0,255,100);
        break;
    case 3:
        color = QColor(0,255,0,100);
        break;
    case 4:
        color = QColor(255,0,0,100);
        break;
    case 5:
        color = QColor(0,0,0,255);
        break;
    }
}

void MacrodynGraphicsItem::zoom(int x1, int x2, int y1, int y2)
{
    if (m_axis && m_origAxis)
    {
        int xmin = qMin(x1, x2);
        int ymin = qMin(y1, y2);
        int xmax = qMax(x1, x2);
        int ymax = qMax(y1, y2);

        int width = (xmax - xmin) * m_supersampling;
        int height = (ymax - ymin) * m_supersampling;

        m_imageMutex.lock();
        if (!m_image->isNull())
        {
            *m_image = m_image->copy((xmin - lmargin) * m_supersampling, (ymin - upmargin) * m_supersampling, width, height).scaled(m_image->size());
        }
        m_imageMutex.unlock();

        update();

        qreal xtemp = pixel_to_x(xmin);
        qreal ytemp = pixel_to_y(ymax);
        m_axis.max[0] = pixel_to_x(xmax);
        m_axis.max[1] = pixel_to_y(ymin);
        m_axis.min[0] = xtemp;
        m_axis.min[1] = ytemp;

        emit axisChanged(m_axis);
    }
    else qDebug() << "axis or origAxis invalid!";
}

void MacrodynGraphicsItem::unzoom()
{
    m_axis = m_origAxis;

    m_imageMutex.lock();
    m_image->fill(m_backgroundColor);
    m_imageMutex.unlock();

    update();
    emit axisChanged(m_axis);
    qDebug() << "unzoom";
}

qreal MacrodynGraphicsItem::getZoom() const
{
    qreal zoom = 1.0;
    if (m_axis && m_origAxis)
    {
        zoom = (m_origAxis.max.at(0) - m_origAxis.min.at(0)) / (m_axis.max.at(0) - m_axis.min.at(0));
    }
    return zoom;
}

void MacrodynGraphicsItem::print()
{
    //emit needRedrawEPS();

//    QPrinter printer(QPrinter::HighResolution);
//    //printer.setOutputFormat(QPrinter::);
//    printer.setPaperSize(QSizeF(80, 80), QPrinter::Millimeter);
//    printer.setOutputFileName("/Users/jtiwi/Documents/build-Macrodyn-Qt-Desktop_Qt_5_3_clang_64bit-Debug/testBild2.eps");
//    QPainter painter(&printer);
//    double xscale = printer.pageRect().width()/double(width());
//    double yscale = printer.pageRect().height()/double(height());
//    double scale = qMin(xscale, yscale);
//    painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
//                       printer.paperRect().y() + printer.pageRect().height()/2);
//    painter.scale(scale, scale);
//    painter.translate(-width()/2, -height()/2);
//    paint(&painter);


    QPrinter printer;
    QPrintDialog dialog(&printer);
    dialog.setWindowTitle(tr("Print Graph"));
    if (dialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        double xscale = printer.pageRect().width()/double(width());
        double yscale = printer.pageRect().height()/double(height());
        double scale = qMin(xscale, yscale);
        painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
                           printer.paperRect().y() + printer.pageRect().height()/2);
        painter.scale(scale, scale);
        painter.translate(-width()/2, -height()/2);

        paint(&painter);
    }
}

void MacrodynGraphicsItem::savePdf(const QString& path)
{
//    QFileDialog dialog;
//    dialog.setWindowModality(Qt::WindowModal);
//    dialog.setAcceptMode(QFileDialog::AcceptSave);
//    dialog.exec();
//    QStringList files = dialog.selectedFiles();

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QSizeF(800, 800), QPrinter::Millimeter);
    printer.setOutputFileName(path);
    QPainter painter(&printer);
    double xscale = printer.pageRect().width()/double(width());
    double yscale = printer.pageRect().height()/double(height());
    double scale = qMin(xscale, yscale);
    painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
                       printer.paperRect().y() + printer.pageRect().height()/2);
    painter.scale(scale, scale);
    painter.translate(-width()/2, -height()/2);
    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);
    paint(&painter);
}
