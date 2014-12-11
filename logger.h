#ifndef Logger_H
#define Logger_H

#include <QObject>
#include <QString>


class LoggerHelper
{

public:

    LoggerHelper();
    ~LoggerHelper();

    LoggerHelper& operator <<(const QString&);
    LoggerHelper& operator <<(qreal n);


protected:

    static int ref;
    QString m_buffer;
};

class Logger : public QObject
{
    friend class LoggerHelper;

    Q_OBJECT

    Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)
    Q_PROPERTY(QString line READ line NOTIFY lineChanged)

public:

    Logger(QObject * = 0);
    ~Logger();

    void print(const QString&);
    void print(qreal, int precision = 0);

    inline QString line() const { return m_oldLine; }
    inline int precision() const { return m_precision; }
    void setPrecision(int);


signals:

    void lineChanged();
    void precisionChanged();


protected:

    QString m_line;
    QString m_oldLine;
    int m_precision;


private:

    static QList<Logger *> m_logger;
};

inline LoggerHelper log() { return LoggerHelper(); }

#endif // Logger_H
