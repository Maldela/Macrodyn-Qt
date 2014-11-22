#ifndef LOG_H
#define LOG_H

#include <QQuickItem>
#include <QString>

#define endl "\n"

class Log : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)

public:

    explicit Log(QQuickItem *parent = 0);
    ~Log() {}

    static Log& log() { return *m_log; }

    void print(const QString&);
    void print(qreal, int precision = 0);

    Log& operator<<(const QString &str) { print(str); return *this; }
    Log& operator<<(qreal n) { print(n); return *this; }

    inline int precision() const { return m_precision; }
    void setPrecision(int);


protected:

    static Log *m_log;

    QString m_line;
    int m_precision;


signals:

    void newLine(QString);
    void precisionChanged();


public slots:

};

#endif // LOG_H
