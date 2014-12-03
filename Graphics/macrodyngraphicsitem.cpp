#include "macrodyngraphicsitem.h"
#include "../Printer/printer.h"

MacrodynGraphicsItem::MacrodynGraphicsItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    image = new QImage();
    lmargin = LMARGIN;
    rmargin = RMARGIN;
    lowmargin = LOWMARGIN;
    upmargin = UPMARGIN;
}



/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: drawAxis                                                  */
/* Purpose:         drawing the axis                                          */
/*                                                                            */
/* This function is responsible for drawing the axis,                         */
/* computing the marks and labeling the axis->                                 */
/* there are 3 possible color options to set:                                 */
/* AXISCOLOR sets the color of the axis                                       */
/* AXISLABELCOLOR sets the color of the string written                        */
/* XICSMARKSCOLOR sets the color of the numbers written                       */
/* MAXLABELLENGTH :max number of chars of the label strings                   */
/*                                                                            */
/* Modified:        1994/1995 (Markus Lohmann, Jorg Nikutta)                  */
/* Last modified:   Mon Jan 31 14:59:06 MET 2000  Michael Meyer               */
/*                                                                            */
/******************************************************************************/
int MacrodynGraphicsItem::drawAxis()
{
    int i;
    int axisMark;
    QString markLabel;
    QString shortxLabel = axis->label.at(0).left(MAXLABELLENGTH + 2);
    QString shortyLabel = axis->label.at(1).left(MAXLABELLENGTH + 2);;
    QString char_chain;
    QPolygon pointsX;
    pointsX << QPoint(0, 0);         /* coords for arrow on x-axis */
    pointsX << QPoint(-6, 3);
    pointsX << QPoint(-6, -3);

    QPolygon pointsY;
    pointsY << QPoint(0, 0);         /* coords for arrow on y-axis */
    pointsY << QPoint(-3, 6);
    pointsY << QPoint(3, 6);


//    /* create Graphics Contexts */
//    axisGC=XCreateGC(base->displ,buffer_pix,0,0);    // GC for drawing the axis
//    axisSupGC=XCreateGC(base->displ,buffer_pix,0,0); // supplemental GC for
//                // drawing figueres & labels


//    /* check if left margin big enough for vertical numbers, if not enlarge */
//    for (i=0;i<=4;i++){
//        sprintf(markLabel,"%-1.4g",axis->min[1]+i*(axis->max[1]-axis->min[1])/4);
//        textWidth=XTextWidth(fontStruct,markLabel,strlen(markLabel));
//        if( (textWidth+12) > (int)lmargin ){//create enough space to hold numbers
//            cerr << "INFORMATION:increase left margin from " << lmargin;
//            lmargin=textWidth+12;       // that means enlarge left margin if
//                            // necessary
//            cerr << "to " << lmargin << endl;
//        }
//    }

    /* Draw Axis + ar(height()) */
    QPainter painter(image);
    painter.setBrush(QBrush(AXISCOLOR));
    painter.drawLine(lmargin, height()-lowmargin, width()-rmargin+5, height()-lowmargin); /* X */
    pointsX.translate(width()-rmargin+10, height()-lowmargin);/*set starting point for x-arrow*/
    /*draw x-arrow */
    painter.drawPolygon(pointsX);

    painter.drawLine(lmargin, height()-lowmargin, lmargin, upmargin-5); /* Y */
    pointsX.translate(lmargin, upmargin-10);/*set starting point for y-arrow*/
    /*draw y-arrow */
    painter.drawPolygon(pointsY);

    // draw zeroline
    if ((axis->zeroline[1].isValid()) && (axis->min[1] < 0))
    {
        int zl = (int)(upmargin+((height())-lowmargin-upmargin) * axis->max[1] / (axis->max[1]-axis->min[1]));
        painter.drawLine(lmargin, zl, width()-rmargin+5, zl);
    }

    painter.setBrush(QBrush(AXISLABELCOLOR));

    /* Put Label on axis */
    painter.drawText(width()-rmargin+2, height()-lowmargin-5, shortxLabel);
    painter.drawText(5, upmargin-7, shortyLabel);

    painter.setBrush(QBrush(XICSMARKSCOLOR));

    /* Draw Marks on horizontal axis */
    for (i=0; i<=4; i++)
    {
        axisMark = i*((width())-lmargin-rmargin)/4;
        markLabel.number(axis->min[0]+i*(axis->max[0]-axis->min[0])/4);
        painter.drawLine(lmargin+axisMark, height()-lowmargin, lmargin+axisMark, height()-lowmargin+5);
        painter.drawText(lmargin+axisMark-10, height()-lowmargin+18, markLabel);
    }

    /* Draw Marks on vertical axis */
    int j,ii;
    if (axis->zeroline[1].isValid() && axis->min[1] < 0)
    {
       int zerow = (int)(((height())-lowmargin-upmargin)*axis->max[1]/(axis->max[1]-axis->min[1]));
       if((axis->max[1]/(-axis->min[1]))>3.8) {i=4;j=1;}
       else if((axis->max[1]/(-axis->min[1]))>1.9) {i=4;j=2;}
       else if((axis->max[1]/(-axis->min[1]))>1.3) {i=3;j=2;}
       else if((axis->max[1]/(-axis->min[1]))>0.76) {i=2;j=2;}
       else if((axis->max[1]/(-axis->min[1]))>0.53) {i=2;j=3;}
       else if((axis->max[1]/(-axis->min[1]))>0.26) {i=2;j=4;}
       else {i=1;j=4;}
       for (ii=0;ii<j;ii++)
       {
           axisMark = ii*(height()-upmargin-lowmargin-zerow)/j;
           painter.drawLine(lmargin-5, height()-lowmargin-axisMark, lmargin, height()-lowmargin-axisMark);
           markLabel.number(axis->min[1]+ii*(-axis->min[1])/j);
           painter.drawText(lmargin-7, height()-lowmargin-axisMark+5, markLabel);
       }
       for (ii=0;ii<=i;ii++)
       {
           axisMark = ii*zerow/i;
           painter.drawLine(lmargin-5, upmargin+zerow-axisMark, lmargin, upmargin+zerow-axisMark);
           markLabel.number(ii*axis->max[1]/i);
           painter.drawText(lmargin-7, upmargin+zerow-axisMark+5, markLabel);
       }
    }
    else {
       for (i=0; i<=4; i++)
       {
           axisMark = i*(height()-upmargin-lowmargin)/4;
           painter.drawLine(lmargin-5, height()-lowmargin-axisMark, lmargin, height()-lowmargin-axisMark);
           markLabel.number(axis->min[1]+i*(axis->max[1]-axis->min[1])/4);
           painter.drawText(lmargin-7, height()-lowmargin-axisMark+5, markLabel);
       }
    }
// draw color ranges for contourline plot into window
// if wished
//TODO
//    if((job==CONTOURLINE)||(job==CONTOURLINE_B)||(job==COLOR_PLOT)||(job==COLOR_PLOT_B))
//    {
//        qreal lval,rval;
//        int cmin=base->cmin;
//        int cmax=base->cmax;
//        qreal smin=axis->min[2]/cmax;
//        qreal smax=axis->max[2]/cmax;
//    //Print color caskets
//        int yPosCount=upmargin-1;
//        int yPosStep=(WINHEIGHT-upmargin-lowmargin)/(cmax+17-cmin+1+1);
//        char_chain = QString("color scale of").append(axis->label[2]);
//        XDrawString(base->displ,buffer_pix,base->ColorGC[29],
//                (width()),yPosCount,
//                char_chain,strlen(char_chain));
//        for (i=cmin;i<=cmax+17;i++)
//        { // paint color map
//            yPosCount+=yPosStep;
//            XFillRectangle(base->displ,buffer_pix,base->ColorGC[i],
//                (width())+50,yPosCount-yPosStep/8,15,9);
//        }
//    // print color range for interval
//        smax=(axis->max[2]-axis->min[2])/(cmax+17);
//        yPosCount=upmargin+5+yPosStep;
//        i=1;
//        lval=axis->max[2]-(i*smax);
//        sprintf(char_chain,"[%4.3f,%4.3f]",lval,axis->max[2]);

//        XFillRectangle(base->displ,buffer_pix,base->ColorGC[29],
//            (width())+35,yPosCount-yPosStep/30,30,1);

//        XDrawString(base->displ,buffer_pix,base->ColorGC[29],
//                (width())-40,yPosCount,
//                char_chain,strlen(char_chain));
//        for(i=2;i<34;i+=2)
//        {
//            yPosCount+=yPosStep*2;
//            rval=axis->max[2]-((i+1)*smax);
//            sprintf(char_chain,"%4.3f",rval);

//            XFillRectangle(base->displ,buffer_pix,base->ColorGC[29],
//                (width())-10,yPosCount-yPosStep/30,75,1);

//            XDrawString(base->displ,buffer_pix,base->ColorGC[29],
//                (width())-40,yPosCount,
//                char_chain,strlen(char_chain));
//        }
//        yPosCount+=yPosStep+3;
//        i=34;
//                lval = axis->max[2]-(i*smax);
//        sprintf(char_chain,"%4.3f",lval);

//        XFillRectangle(base->displ,buffer_pix,base->ColorGC[29],
//            (width())-10,yPosCount-yPosStep/30,75,1);

//        XDrawString(base->displ,buffer_pix,base->ColorGC[29],
//            (width())-40,yPosCount+2,
//            char_chain,strlen(char_chain));
//    }
    return(0);
}

///////////////////////////////////////////////////////////////////////////////
// Class name:          graphics
// Member function:	draw_mp_names
// Purpose:		write multiple names in the window
//
// Author:		Marc Mueller May 30 1999
// Last modified:	Sun May 30 23:47:21 CEST 1999 Marc Mueller
///////////////////////////////////////////////////////////////////////////////
void MacrodynGraphicsItem::draw_mp_names(const QList<QString>& names)
{
    QString string;
    for (int j=0;j<names.size();j++)
    {
        string.append(names.at(j));
        string.append(" ");
    }
    QRect rect(QPoint(lmargin+10, upmargin-5), QPoint(width()-rmargin-10, height()-lowmargin));
    QPainter painter(image);
    //TODO
    //painter.setBrush(QBrush(QColor(jnngfvtdxljn)));
}

///////////////////////////////////////////////////////////////////////////////
// Class name:          graphics
// Member function:	drawString
// Purpose:		write a string in the window
//
// Author:		Andreas Starke
// Last modified:
///////////////////////////////////////////////////////////////////////////////
void MacrodynGraphicsItem::drawString(const qreal& x,const qreal& y, const QString& text, const QColor& color)
{
    int pixX, pixY; // pixel coordinates
    getCoordinates(x,y,pixX,pixY);
    QPainter painter(image);
    painter.setBrush(QBrush(color));
    painter.drawText(pixX, pixY, text);
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
//    int max=0;
//    int paint_size;
//    int cmax=base->cmax;
//    int cmin=base->cmin;
//    int size=(base->win_width)-(width())-rmargin-2;
//    int yPosStep;
//    int yPosCount;

//    if ((axis->min[2]<0)&&(axis->max[2]>0))
//    {
//        yPosStep=(WINHEIGHT-upmargin-lowmargin)/(cmax+17-cmin+2);
//        yPosCount=upmargin+yPosStep;
//        for(i=cmin;i<=cmax+17;i++) {
//            if(colorCount[i]>max) max=colorCount[i];
//        }
//        for(i=cmin;i<=cmax+17;i++) {
//            paint_size=2*(colorCount[i]*size)/(3*max);
//            if(paint_size>0)
//                XFillRectangle(base->displ,buffer_pix,base->ColorGC[i],
//                    (width())+70,yPosCount,paint_size,3);
//            yPosCount+=yPosStep;
//        }
//    }
//    if((axis->max[2]>0) && (axis->min[2]>=0)){
//        yPosStep=(WINHEIGHT-upmargin-lowmargin)/(cmax+17-cmin+2);
//        yPosCount=upmargin+yPosStep;
//        for(i=cmin;i<=cmax;i++) {
//            if(colorCount[i]>max) max=colorCount[i];
//        }
//        for(i=cmin;i<=cmax;i++) {
//            paint_size=2*(colorCount[i]*size)/(3*max);
//            if(paint_size>0)
//                XFillRectangle(base->displ,buffer_pix,base->ColorGC[i],
//                    (width())+70,yPosCount,paint_size,3);
//            yPosCount+=yPosStep;
//        }
//    }
//    if((axis->max[2]<=0) && (axis->min[2]<0)){
//        yPosStep=(WINHEIGHT-upmargin-lowmargin)/(cmax+17-cmin+2);
//        yPosCount=upmargin+yPosStep+120;
//        for(i=cmin+17;i<=cmax+17;i++) {
//            if(colorCount[i]>max) max=colorCount[i];
//        }
//        for(i=cmin+17;i<=cmax+17;i++) {
//            paint_size=2*(colorCount[i]*size)/(3*max);
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
// purpose:		set min and max value of axis
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////

void MacrodynGraphicsItem::set_axis(short which, qreal to_min, qreal to_max)
{
    axis->min[which] = to_min;
    axis->max[which] = to_max;
}

///////////////////////////////////////////////////////////////////////////////
//
// member function:	get_axis
// purpose:		get min and max value of axis
//
// author:		Andreas Starke
// last modified:
//
///////////////////////////////////////////////////////////////////////////////

void MacrodynGraphicsItem::get_axis(short which, qreal *to_min, qreal *to_max)
{
    *to_min = axis->min[which];
    *to_max = axis->max[which];
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
    delete image;
    image = new QImage(boundingRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
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
void MacrodynGraphicsItem::setPoint(const qreal& v, const qreal& w, const QColor& color)
{
    int pixv, pixw, x, y; /* coordinates in pixels */

    getCoordinates(v,w,x,y);

    pixv = (int)lmargin + x;
    pixw = (height()) - ((int)lowmargin + y );
    if (pixv <= (int)lmargin || pixv > ((width()) - (int)rmargin) ||
        pixw < (int)upmargin || pixw >= ((height()) - (int)lowmargin) )
    {
    }
//	cerr << "Warning: Not available pixel: " << pixv << " " << pixw << endl;
    else
    {
        QPainter painter(image);
        painter.setBrush(QBrush(color));
        painter.drawPoint(pixv, pixw);
    }
}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: setBigPoint                                               */
/* Purpose:         draw a point on the screen with variable size             */
/* Modified:        mhoffmann                  			              */
/* Last modified:   Mon Sep 11 12:00:14 METDST 2000                           */
/*                                                                            */
/******************************************************************************/
void MacrodynGraphicsItem::setBigPoint(const qreal& v, const qreal& w, const QColor& color, int size)
{
    int pixv, pixw, x, y; /* coordinates in pixels */

    getCoordinates(v,w,x,y);

    pixv = (int)lmargin + x;
    pixw = (height()) - ((int)lowmargin + y );
    if (pixv <= (int)lmargin || pixv > ((width()) - (int)rmargin) ||
        pixw < (int)upmargin || pixw >= ((height()) - (int)lowmargin) )
    {
    }
//	cerr << "Warning: Not available pixel: " << pixv << " " << pixw << endl;
    else
    {
        QRect rect(pixv-size/2, pixw-size/2, size, size);
        QPainter painter(image);
        painter.fillRect(rect, color);
    }

}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: drawLine                                                  */
/* Purpose:         draw a line on the screen                                 */
/* Last modified:   18.01.2005 (Andreas Starke)                               */
/*                                                                            */
/******************************************************************************/
void MacrodynGraphicsItem::drawLine(const qreal& x0, const qreal& y0,
            const qreal& x1, const qreal& y1, const QColor& color)
{
    int pixX0,pixY0,pixX1,pixY1; // pixel coordinates

    qreal x2,y2,x3,y3;
    x2=x0;
    y2=y0;
    x3=x1;
    y3=y1;
    if ( (x0>axis->max[0]) || (x1>axis->max[0]) || (x0<axis->min[0]) || (x1<axis->min[0])
         || (y0>axis->max[1]) || (y1>axis->max[1]) || (y0<axis->min[1]) || (y1<axis->min[1]) )
    {
        if ( (y0>axis->max[1]) && (y1<=axis->max[1]) ){
            qreal a,b;
            a=y0;
            b=(y1-y0)/(x1-x0);
            x2=(axis->max[1]-a)/b+x0;
            y2=axis->max[1];
        }
        if ( (y1>axis->max[1]) && (y0<=axis->max[1]) ){
            qreal a,b;
            a=y0;
            b=(y1-y0)/(x1-x0);
            x3=(axis->max[1]-a)/b+x0;
            y3=axis->max[1];
        }
        if ( (y0<axis->min[1]) && (y1>=axis->min[1]) ){
            qreal a,b;
            a=y0;
            b=(y1-y0)/(x1-x0);
            x2=(axis->min[1]-a)/b+x0;
            y2=axis->min[1];
        }
        if ( (y1<axis->min[1]) && (y0>=axis->min[1]) ){
            qreal a,b;
            a=y0;
            b=(y1-y0)/(x1-x0);
            x3=(axis->min[1]-a)/b+x0;
            y3=axis->min[1];
        }
        if ( (x0>axis->max[0]) && (x1<=axis->max[0]) ){
            qreal a,b;
            a=x0;
            b=(x1-x0)/(y1-y0);
            y2=(axis->max[0]-a)/b+y0;
            x2=axis->max[0];
        }
        if ( (x1>axis->max[0]) && (x0<=axis->max[0]) ){
            qreal a,b;
            a=x0;
            b=(x1-x0)/(y1-y0);
            y3=(axis->max[0]-a)/b+y0;
            x3=axis->max[0];
        }
        if ( (x0<axis->min[0]) && (x1>=axis->min[0]) ){
            qreal a,b;
            a=x0;
            b=(x1-x0)/(y1-y0);
            y2=(axis->min[0]-a)/b+y0;
            x2=axis->min[0];
        }
        if ( (x1<axis->min[0]) && (x0>=axis->min[0]) ){
            qreal a,b;
            a=x0;
            b=(x1-x0)/(y1-y0);
            y3=(axis->min[0]-a)/b+y0;
            x3=axis->min[0];
        }
    }
    //cout << "coordinates: ("<<x2<<","<<y2<<") ("<<x3<<","<<y3<<")"<<endl;
    getCoordinates(x2,y2,pixX0,pixY0);
    getCoordinates(x3,y3,pixX1,pixY1);


    pixX0 += (int)lmargin;
    pixX1 += (int)lmargin;
    pixY0 = (height()) - ((int)lowmargin + pixY0 );
    pixY1 = (height()) - ((int)lowmargin + pixY1 );
    if (pixX0 <= (int)lmargin ||
        pixX0 > ((width()) - (int)rmargin) ||
        pixY0 < (int)upmargin ||
        pixY0 >= ((height()) - (int)lowmargin) )
    {
        Log::log() << "Warning: Not available pixel: " << pixX0 << " " << pixY0 << "\n";
        return;
    }
    if( pixX1 <= (int)lmargin ||
        pixX1 > ((width()) - (int)rmargin) ||
        pixY1 < (int)upmargin ||
        pixY1 >= ((height()) - (int)lowmargin) )
    {
        Log::log() << "Warning: Not available pixel: " << pixX1 << " " << pixY1 << "\n";
        return;
    }
    QPainter painter(image);
    painter.setBrush(QBrush(color));
    painter.drawLine(pixX0, pixY0, pixX1, pixY1);
}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: getCoordinates                                            */
/* Purpose:         transformation of the point coordinates into pixels       */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/
void MacrodynGraphicsItem::getCoordinates(qreal v,qreal w,int& x,int& y)
{
    x=(int)((v-axis->min[0])*(wid-1)/(axis->max[0]-axis->min[0])+0.5)+1;
    y=(int)((w-axis->min[1])*(hig-1)/(axis->max[1]-axis->min[1])+0.5)+1;
//    if( w != 1 )
//	printf("%22.20lf\n", w);
}


void MacrodynGraphicsItem::dumpGraphics(const QString& fileName) const
{
    QFile file(fileName);
    if (file.exists()) Log::log() << "Overwriting file " << fileName;
    if (!file.open(QIODevice::WriteOnly))
    {
        Log::log() << "Can't open file " << fileName;
        return;
    }
    QDataStream stream(&file);
    stream << *image;
    Log::log() << "Graph stored in " << fileName;
}

/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: clearColumn                                               */
/* Purpose:         clears a column of the graphics window according to the   */
/*                  input                                                     */
/* Last modified:   17.10.1994 (Markus Lohmann)                               */
/*                                                                            */
/******************************************************************************/
void MacrodynGraphicsItem::clearColumn(qreal x)
{
    int col;				// coordinate of the column to be
                    // cleared
    int row;				// 0 <= row <= height of the window

    getCoordinates(x, 0.0, col, row);	// transformation of x into pixel
                    // coordinates
    col += lmargin;
    QPainter painter(image);
    painter.setBrush(QBrush(backgroundColor));
    painter.drawLine(col, 0, col, image->height());
                        // draw the hole column in the
                    // background color
}
/******************************************************************************/
/*                                                                            */
/* Class name:      graphics                                                  */
/* Member function: reset                                                     */
/* Purpose:         resets the domain under consideration that should be      */
/*                  displayed on the screen                                   */
/* Last modified:   02.08.1995 (Markus Lohmann)                               */
/* Modified:        redraw of axis inserted 02/08/95 (ML)                     */
/*                                                                            */
/******************************************************************************/
void MacrodynGraphicsItem::reset(const xyRange& newDomain)
{
    *axis = newDomain;
    lmargin = LMARGIN;
    rmargin = RMARGIN;
    lowmargin = LOWMARGIN;
    upmargin = UPMARGIN;
    delete image;
    image = new QImage(width(), height(), QImage::Format_ARGB32_Premultiplied);
    drawAxis();
    wid = (width()) - lmargin - rmargin;
    hig = (height()) - upmargin - lowmargin;
    right = wid + lmargin;
    down = hig + upmargin;
}


/*******************************
 * draw new
 * zoom rectangle
 * from point (xinit, yinit)
 * to current point: (xcurr, ycurr)
 *******************************/
void MacrodynGraphicsItem::draw_zoom_rect(int& xcurr, int& ycurr)
{
    int  rect_width, rect_height;

    /* avoid drawing out of drawable area */
    if (xcurr < (int)lmargin) xcurr = (int)lmargin;
    if (ycurr < (int)upmargin) ycurr = (int)upmargin;
    if (xcurr > (int)right) xcurr = right;
    if (ycurr > (int)down) ycurr = down;

    /* ordering of values */

    if (ycurr > (int)yinit)
    {
        ypix_Max = ycurr;
        ypix_Min = yinit;
    }
    else
    {
        ypix_Max = yinit;
        ypix_Min = ycurr;
    }
    if (xcurr > (int)xinit)
    {
        xpix_Max = xcurr;
        xpix_Min = xinit;
    }
    else
    {
        xpix_Max = xinit;
        xpix_Min = xcurr;
    }
    zoomRect.setLeft(xpix_Min);
    zoomRect.setTop(ypix_Min);

    rect_width = xpix_Max-xpix_Min;
    rect_height = ypix_Max-ypix_Min;

    zoomRect.setWidth(rect_width);
    zoomRect.setHeight(rect_height);
}

/*********************************
 * function to convert
 * pixel values on x-values
 *********************************/
double MacrodynGraphicsItem::pixel_to_x(int xpix)
{
    return (xpix-lmargin) * (axis->max[0] - axis->min[0]) / wid + axis->min[0];
}
/*********************************
 * function to convert
 * pixel values on y-values
 *********************************/
double MacrodynGraphicsItem::pixel_to_y(int ypix)
{
    return (height()-lowmargin-ypix) * (axis->max[1]-axis->min[1]) / hig + axis->min[1];
}

void MacrodynGraphicsItem::setBackgroundColor(const QColor& c)
{
    if (backgroundColor != c)
    {
        backgroundColor = c;
        emit backgroundColorChanged();
    }
}
