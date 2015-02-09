#ifndef MACRODYNGRAPHICSITEM_H
#define MACRODYNGRAPHICSITEM_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QPair>
#include <QReadWriteLock>
#include <QThread>

#include "../sim.h"
#include "../axes.h"


//TODO:
// - draw image
// - draw zoomRect
// - beautify setBigPoint
// - drawColorCount?
class MacrodynGraphicsItem;

class ImagePainter : public QObject
{
    Q_OBJECT

public:

    ImagePainter(MacrodynGraphicsItem *, QImage *, QReadWriteLock *, QReadWriteLock *);


signals:

    void imageChanged();
    void imageFinished();


protected slots:

    void drawPoint(const QPointF&, const QColor&, bool = true);
    void drawRect(const QRectF&, const QColor&, bool = true);
    void drawLine(const QLineF&, const QColor&, bool = true);
    void redraw();


protected:

    QImage *m_image;
    QReadWriteLock *m_imageLock, *m_listLock;
    MacrodynGraphicsItem *m_parent;
};


class MacrodynGraphicsItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(qreal zoomFactor READ getZoom NOTIFY zoomChanged)

    friend class ImagePainter;

public:

    explicit MacrodynGraphicsItem(QQuickItem *parent = 0);
    virtual ~MacrodynGraphicsItem();

    void drawAxis(QPainter * = NULL);                     // draw axis according to the domain

    void draw_mp_names(const QStringList&);// write multiple names in the window
//    void draw_color_count();	// Job color_map
    void set_axis(int, qreal, qreal);	// set qMax & qMin of axis
    void get_axis(int, qreal*, qreal*); // get qMax & qMin of axis
    void clear_window();		// clear output window

    void setPoint(qreal, qreal, QColor color);
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
    inline QColor getBackgroundColor() const { return backgroundColor; }
    void setBackgroundColor(const QColor&);
    qreal getZoom() const;
//    QColor get_color(int) const;

    void clearColumn(qreal);	// clear a specified column of the
                    // output window
    void closeGraphics();
    void setXYRange(const xyRange& range);

    void colorFromInt(QColor& color, int colorInt) const;
    int transformX(qreal) const;
    int transformY(qreal) const;
    QPoint transform(const QPointF&) const;
    double pixel_to_x(int) const;
    double pixel_to_y(int) const;


public slots:

    Q_INVOKABLE void zoom(int, int, int, int);
    Q_INVOKABLE inline void unzoom() { axis = origAxis; emit needRedraw(); qDebug() << "unzoom"; }
    Q_INVOKABLE void click(int x, int y) const { qDebug() << QPointF(pixel_to_x(x), pixel_to_y(y)); }


signals:

    void backgroundColorChanged();
    void zoomChanged();
    void newPoint(QPointF, QColor);
    void newRect(QRectF, QColor);
    void newLine(QLineF, QColor);
    void needRedraw();


protected slots:

    void handleSizeChanged();


protected:

    void paint(QPainter *painter);

    QImage image, backupImage;
    ImagePainter *m_imagePainter;
    QThread *imageThread;
    QList<QPair<QLineF, QColor> > m_lines;
    QList<QPair<QPointF, QColor> > m_points;
    QList<QPair<QRectF, QColor> > m_rects;    QTimer redrawTimer;
    QReadWriteLock lock, imageLock;
    int job;
    xyRange origAxis;
    xyRange axis;
    QColor backgroundColor;
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

    qint64 colorCount[95];
};

#endif // MACRODYNGRAPHICSITEM_H
