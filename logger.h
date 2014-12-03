#ifndef Logger_H
#define Logger_H

#include <QObject>
#include <QString>

#define endl "\n"

class LoggerHelper
{

public:

    LoggerHelper& operator <<(const QString&);
    LoggerHelper& operator <<(qreal n);
};

class Logger : public QObject
{
    friend class LoggerHelper;

    Q_OBJECT

    Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)
    Q_PROPERTY(QString line READ line NOTIFY lineChanged)

public:

    explicit Logger(QObject * = 0);
    ~Logger();

    void print(const QString&);
    void print(qreal, int precision = 0);

    inline QString line() const { return m_oldLine; }
    inline int precision() const { return m_precision; }
    void setPrecision(int);


protected:

    QString m_line;
    QString m_oldLine;
    int m_precision;


private:

    static QList<Logger *> m_logger;


signals:

    void lineChanged();
    void precisionChanged();


public slots:

};

inline LoggerHelper log() { return LoggerHelper(); }

#endif // Logger_H
