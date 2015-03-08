#ifndef MACRODYNGRAPHICSITEM_H
#define MACRODYNGRAPHICSITEM_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QPair>
#include <QReadWriteLock>
#include <QMutex>
#include <QThread>

#include "../sim.h"
#include "../axes.h"


//TODO:
// - beautify setBigPoint
// - drawColorCount?
struct MacroString
{
    MacroString(const QString& s, const QPointF& p, const QColor& c, bool t)
    {
        string = s;
        point = p;
        color = c;
        transform = t;
    }

    QString string;
    QPointF point;
    QColor color;
    bool transform;
};

class MacrodynGraphicsItem;

class ImagePainter : public QObject
{
    Q_OBJECT

public:

    ImagePainter(MacrodynGraphicsItem *, QImage *, QReadWriteLock *, QMutex *);
    virtual ~ImagePainter();


signals:

    void imageChanged();
    void imageFinished(QImage *);


protected slots:

    void drawPoint(const QPointF&, const QColor&, bool = false);
    void drawRect(const QRectF&, const QColor&, bool = false);
    void drawLine(const QLineF&, const QColor&, bool = false);
    void drawString(const QPointF&, const QString&, const QColor&, bool, bool = false);
    void clearColumn(qreal, bool = false);
    void redraw();
    void redrawEPS();


protected:

    QImage *m_image, *m_parentImage;
    QReadWriteLock *m_listLock;
    QMutex *m_imageMutex;
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

    void draw_mp_names(const QStringList&);// write multiple names in the window
//    void draw_color_count();	// Job color_map
    void set_axis(int, qreal, qreal);	// set max & min of axis
    void get_axis(int, qreal*, qreal*) const; // get max & min of axis
    void clear_window();		// clear output window
    void setPoint(qreal, qreal, const QColor&);
                    // highlights a pixel on the screen
                    // according to the given point
    void setBigPoint(qreal, qreal, const QColor&, int);
    void setBigPoint(qreal, qreal, int, int);
    void setRect(qreal x, qreal w, qreal width, qreal height, const QColor& color);
    void setLine(qreal, qreal, qreal, qreal, int); // draw a line on the screen
    void setString(qreal, qreal, const QString&, const QColor&, bool = true);  // should be displayed on the screen
    inline QColor getBackgroundColor() const { return backgroundColor; }
    void setBackgroundColor(const QColor&);
    qreal getZoom() const;
//    QColor get_color(int) const;
    void clearColumn(qreal);	// clear a specified column of the output window
    void setXYRange(const xyRange& range);
    void colorFromInt(QColor& color, int colorInt) const;
    int transformX(qreal) const;
    int transformY(qreal) const;
    QPoint transform(const QPointF&) const;
    double pixel_to_x(int, bool = true) const;
    double pixel_to_y(int, bool = true) const;


public slots:

    Q_INVOKABLE void zoom(int, int, int, int);
    Q_INVOKABLE inline void unzoom() { axis = origAxis; emit needRedraw(); qDebug() << "unzoom"; }
    Q_INVOKABLE void click(int x, int y) const { qDebug() << QPointF(pixel_to_x(x), pixel_to_y(y)); }
    Q_INVOKABLE void print();

    Q_INVOKABLE void savePdf(const QString&);


signals:

    void backgroundColorChanged();
    void zoomChanged();
    void newPoint(QPointF, QColor);
    void newRect(QRectF, QColor);
    void newLine(QLineF, QColor);
    void newClearColumn(qreal);
    void newString(QPointF, QString, QColor, bool);
    void needRedraw();
    void needRedrawEPS();


protected slots:

    void handleSizeChanged();
    void newImage(QImage *);


protected:

    void paint(QPainter *painter);
    void drawAxis(QPainter * = NULL);    // needs locking of axisLock before calling!

    QImage *image;
    ImagePainter *m_imagePainter;
    QThread imageThread;
    QList<QPair<QLineF, QColor> > m_lines;
    QList<QPair<QPointF, QColor> > m_points;
    QList<QPair<QRectF, QColor> > m_rects;
    QList<qreal> m_clearColumns;
    QList<MacroString> m_strings;
    mutable QReadWriteLock listLock, axisLock;
    mutable QMutex imageMutex;
    int job;
    xyRange origAxis;
    xyRange axis;
    QColor backgroundColor;
    int rmargin;
    int lmargin;
    int upmargin;
    int lowmargin;
    int wid;
    int hig;
    int xinit, yinit, xpix_min, ypix_min, xpix_max, ypix_max;
    int right;
    int down;
    int bigPointSize;

    qint64 colorCount[95];
};

#endif // MACRODYNGRAPHICSITEM_H
