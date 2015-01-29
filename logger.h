#ifndef Logger_H
#define Logger_H

#include <QObject>
#include <QString>

namespace MacrodynError
{
    enum type
    {
        noError,
        normal,
        fatal
    };
}

class LoggerHelper
{

public:

    LoggerHelper(enum MacrodynError::type = MacrodynError::noError);
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
    void errorOccured() const;
    void fatalErrorOccured() const;


protected:

    inline void error(enum MacrodynError::type e) const
    {
        if (e == MacrodynError::normal) emit errorOccured();
        if (e == MacrodynError::fatal) emit fatalErrorOccured();
    }

    QString m_line;
    QString m_oldLine;
    int m_precision;


private:

    static QList<Logger *> m_logger;
};


inline LoggerHelper log() { return LoggerHelper(); }

inline void fatalError(const QString &where, const QString &what = "")
{
    LoggerHelper(MacrodynError::fatal) << "Fatal error:" << where << ":" << what;
}

inline void error(const QString &arg1, const QString &arg2 = "")
{
    LoggerHelper(MacrodynError::normal) << "Error:" << arg1 << ":" << arg2;
}

#endif // Logger_H
