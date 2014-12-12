#ifndef MACRODYNGRAPHICSITEM_H
#define MACRODYNGRAPHICSITEM_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QTimer>

#include "../sim.h"
#include "../axes.h"


//TODO:
// - draw image
// - draw zoomRect
// - beautify setBigPoint
// - drawColorCount?


class MacrodynGraphicsItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

public:

    explicit MacrodynGraphicsItem(QQuickItem *parent = 0);
    virtual ~MacrodynGraphicsItem();

    int drawAxis();                     // draw axis according to the domain

    void draw_mp_names(const QStringList&);// write multiple names in the window
//    void draw_color_count();	// Job color_map
    void set_axis(int, qreal, qreal);	// set qMax & qMin of axis
    void get_axis(int, qreal*, qreal*); // get qMax & qMin of axis
    void clear_window();		// clear output window

    void setPoint(const qreal&, const qreal&, const QColor&);
                    // highlights a pixel on the screen
                    // according to the given point

    void setBigPoint(const qreal&, const qreal&, const QColor&, int);
    void drawLine(const qreal&, const qreal&, const qreal&, const qreal&, const QColor&);
                        // draw a line on the screen
    void drawString(const qreal&, const qreal&, const QString&, const QColor&, bool = true);
    void reset(const xyRange&);         // reset domain under consideration that
                    // should be displayed on the screen
    void dumpGraphics(const QString&) const; // dump output window
    QColor getBackgroundColor() const { return backgroundColor; }
    void setBackgroundColor(const QColor&);
    QColor get_color(int);

    void clearColumn(qreal);	// clear a specified column of the
                    // output window
    void closeGraphics();
    void setXYRange(const xyRange& range);


signals:

    void backgroundColorChanged();


protected slots:

    void handleSizeChanged();
    void resizeImage();


protected:

    void paint(QPainter *painter);

    QImage image;
    QTimer timer;
    int job;
    xyRange axis;
    QColor backgroundColor;
    QRect zoomRect;
    uint lmargin;
    uint rmargin;
    uint upmargin;
    uint lowmargin;
    uint wid;
    uint hig;
    uint xinit, yinit, xpix_min, ypix_min, xpix_max, ypix_max;
    uint right;
    uint down;
    void getCoordinates(qreal, qreal, int&, int&);
    void draw_zoom_rect(int&, int&);
    double pixel_to_x(int);
    double pixel_to_y(int);

    qint64 colorCount[95];
};

#endif // MACRODYNGRAPHICSITEM_H
