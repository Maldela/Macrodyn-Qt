#ifndef MACRODYNGRAPHICSITEM_H
#define MACRODYNGRAPHICSITEM_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QPair>

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

    void setPoint(qreal, qreal, const QColor&);
                    // highlights a pixel on the screen
                    // according to the given point

    void setBigPoint(qreal, qreal, const QColor&, int);
    void setBigPoint(qreal, qreal, int, int);
    void setRect(qreal x, qreal w, qreal width, qreal height, const QColor& color);
    void setLine(qreal, qreal, qreal, qreal, int);
                        // draw a line on the screen
    void drawString(qreal, qreal, const QString&, const QColor&, bool = true);
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

    void colorFromInt(QColor& color, int colorInt);


public slots:

    Q_INVOKABLE void redraw();


signals:

    void backgroundColorChanged();


protected slots:

    void handleSizeChanged();
    void resizeImage();


protected:

    void paint(QPainter *painter);
    void drawPoint(QPair<QPointF, QColor>);
    void drawRect(QPair<QRectF, QColor>);
    void drawLine(QPair<QLineF, QColor>);

    QImage image;
    QTimer timer;
    int job;
    xyRange axis;
    QColor backgroundColor;
    QRect zoomRect;
    QList<QPair<QLineF, QColor> > m_lines;
    QList<QPair<QPointF, QColor> > m_points;
    QList<QPair<QRectF, QColor> > m_rects;
    uint lmargin;
    uint rmargin;
    uint upmargin;
    uint lowmargin;
    uint wid;
    uint hig;
    uint xinit, yinit, xpix_min, ypix_min, xpix_max, ypix_max;
    uint right;
    uint down;
    uint bigPointSize;
    int transformX(qreal);
    int transformY(qreal);
    QPoint transform(const QPointF&);
    void draw_zoom_rect(int&, int&);
    double pixel_to_x(int);
    double pixel_to_y(int);

    qint64 colorCount[95];
};

#endif // MACRODYNGRAPHICSITEM_H
