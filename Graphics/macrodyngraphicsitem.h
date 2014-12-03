#ifndef MACRODYNGRAPHICSITEM_H
#define MACRODYNGRAPHICSITEM_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>


#include "../sim.h"
#include "../axes.h"

#define LMARGIN 40
#define LOWMARGIN 20
#define RMARGIN 60
#define UPMARGIN 20
#define MAXLABELLENGTH 8
#define AXISCOLOR QColor(Qt::black)
#define AXISLABELCOLOR QColor(Qt::red)
#define XICSMARKSCOLOR QColor(Qt::darkGreen)

//TODO:
// - draw image
// - draw zoomRect
// - beautify setBigPoint
// - drawColorCount?


class MacrodynGraphicsItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QObject *pointer READ pointer)

public:

    explicit MacrodynGraphicsItem(QQuickItem *parent = 0);
    virtual ~MacrodynGraphicsItem() { delete image; }

    int drawAxis();                     // draw axis according to the domain

    void draw_mp_names(const QList<QString>&);// write multiple names in the window
//    void draw_color_count();	// Job color_map
    void set_axis(short, qreal, qreal);	// set max & min of axis
    void get_axis(short, qreal*, qreal*); // get max & min of axis
    void clear_window();		// clear output window

    void setPoint(const qreal&, const qreal&, const QColor&);
                    // highlights a pixel on the screen
                    // according to the given point

    void setBigPoint(const qreal&, const qreal&, const QColor&, int);
    void drawLine(const qreal&, const qreal&, const qreal&, const qreal&, const QColor&);
                        // draw a line on the screen
    void drawString(const qreal&, const qreal&, const QString&, const QColor&);
    void reset(const xyRange&);         // reset domain under consideration that
                    // should be displayed on the screen
    void dumpGraphics(const QString&) const; // dump output window
    QColor getBackgroundColor() const { return backgroundColor; }
    void setBackgroundColor(const QColor&);
    QColor get_color(short);

    void clearColumn(qreal);	// clear a specified column of the
                    // output window
    void closeGraphics();
    QObject *pointer() { return qobject_cast<QObject *>(this); }


signals:

    void backgroundColorChanged();


public slots:


protected:

    QImage *image;
    int job;
    xyRange *axis;
    QColor backgroundColor;
    QRect zoomRect;
    quint32 lmargin;
    quint32 rmargin;
    quint32 upmargin;
    quint32 lowmargin;
    quint32 wid;
    quint32 hig;
    quint32 xinit, yinit, xpix_Min, ypix_Min, xpix_Max, ypix_Max;
    quint32 right;
    quint32 down;
    void getCoordinates(qreal, qreal, int&, int&);
    void draw_zoom_rect(int&, int&);
    double pixel_to_x(int);
    double pixel_to_y(int);

    long colorCount[95];
};

#endif // MACRODYNGRAPHICSITEM_H
