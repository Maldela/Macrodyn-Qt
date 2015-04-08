#ifndef MACRODYNGRAPHICSITEM_H
#define MACRODYNGRAPHICSITEM_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QPair>
#include <QReadWriteLock>
#include <QMutex>
#include <QThread>
#include <QSharedPointer>

#include "../sim.h"
#include "../axes.h"
#include "macrostring.h"
#include "imagepainter.h"

Q_DECLARE_METATYPE(QSharedPointer<QImage>)
Q_DECLARE_METATYPE(xyRange)


class ImagePainter;

class MacrodynGraphicsItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(qreal zoomFactor READ getZoom NOTIFY zoomChanged)
    Q_PROPERTY(bool redrawing READ redrawing NOTIFY redrawingChanged)
    Q_PROPERTY(qreal bigPointSize READ bigPointSize WRITE setBigPointSize NOTIFY bigPointSizeChanged)
    Q_PROPERTY(qreal supersampling READ supersampling WRITE setSupersampling NOTIFY supersamplingChanged)

    friend class ImagePainter;

public:

    explicit MacrodynGraphicsItem(QQuickItem *parent = 0);
    virtual ~MacrodynGraphicsItem();

    void draw_mp_names(const QStringList&);// write multiple names in the window
//    void draw_color_count();	// Job color_map
    void set_axis(int, qreal, qreal);	// set max & min of axis
    void get_axis(int, qreal*, qreal*) const; // get max & min of axis
    void clear_window();		// clear output window
    void setPoint(qreal, qreal, const QColor&);                                         //Threadsafe
                    // highlights a pixel on the screen
                    // according to the given point
    void setBigPoint(qreal, qreal, const QColor&, int);                                 //Threadsafe
    void setBigPoint(qreal, qreal, int, int);                                           //Threadsafe
    void setRectangularBigPoint(qreal v, qreal w, const QColor& color, int size);
    void setRect(qreal x, qreal w, qreal width, qreal height, const QColor& color);     //Threadsafe
    void setLine(qreal, qreal, qreal, qreal, int); // draw a line on the screen         //Threadsafe
    void setString(qreal, qreal, const QString&, const QColor&, bool = true);           //Threadsafe
    inline QColor getBackgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor&);
    qreal getZoom() const;
//    QColor get_color(int) const;
    void clearColumn(qreal);	// clear a specified column of the output window
    void setXYRange(const xyRange& range);
    void colorFromInt(QColor& color, int colorInt) const;
    double pixel_to_x(int) const;
    double pixel_to_y(int) const;
    QSize sizeMargined() const { return QSize(wid, hig); }
    bool redrawing() const { return m_redrawing; }
    qreal bigPointSize() const { return m_bigPointSize; }
    void setBigPointSize(qreal size) { if (size != m_bigPointSize) { m_bigPointSize = size; emit bigPointSizeChanged(size); } }
    qreal supersampling() const { return m_supersampling; }
    void setSupersampling(qreal super) { if (super != m_supersampling) { m_supersampling = super; emit supersamplingChanged(super); } }


public slots:

    Q_INVOKABLE void zoom(int, int, int, int);
    Q_INVOKABLE void unzoom();
    Q_INVOKABLE void click(int x, int y) const { qDebug() << QPointF(pixel_to_x(x), pixel_to_y(y)); }
    Q_INVOKABLE void print();
    void setSimulating(bool sim) { m_simulating = sim; }

    Q_INVOKABLE void savePdf(const QString&);


signals:

    void backgroundColorChanged(const QColor&);
    void zoomChanged();
    void needRedraw();
    void axisChanged(const xyRange&);
    void sizeChanged(const QSize&, bool);
    void redrawingChanged();
    void needRedrawEPS();
    void bigPointSizeChanged(qreal);
    void supersamplingChanged(qreal);


protected slots:

    void handleSizeChanged();
    void updateRedrawing(bool redrawing) { if (redrawing != m_redrawing) { m_redrawing = redrawing; emit redrawingChanged(); } }


protected:

    void paint(QPainter *painter);
    void drawAxis(QPainter * = NULL);
    void setImage(QSharedPointer<QImage> image); //Threadsafe

    QSharedPointer<QImage> m_image;
    ImagePainter *m_imagePainter;
    QThread m_imageThread;
    QList<QPair<QLineF, QColor> > m_lines;
    QList<QPair<QPointF, QColor> > m_points, m_bigPoints;
    QList<QPair<QRectF, QColor> > m_rects;
    QList<qreal> m_clearColumns;
    QList<MacroString> m_strings;
    mutable QReadWriteLock m_listLock;
    mutable QMutex m_imageMutex, m_redrawingMutex;
    int m_job;
    xyRange m_origAxis;
    xyRange m_axis;
    QColor m_backgroundColor;
    int rmargin;
    int lmargin;
    int upmargin;
    int lowmargin;
    int pdfTextMarginx;  //additional margin for the axis drawing into .pdf
    int pdfTextMarginy;
    int wid;
    int hig;
    int xinit, yinit, xpix_min, ypix_min, xpix_max, ypix_max;
    int right;
    int down;
    int m_bigPointSize;
    bool m_redrawing, m_simulating;
    qreal m_supersampling;

    qint64 colorCount[95];
};

#endif // MACRODYNGRAPHICSITEM_H
