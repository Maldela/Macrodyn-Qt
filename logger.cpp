#include "logger.h"
#include <QDebug>


int LoggerHelper::ref = 0;

QList<Logger *> Logger::m_logger = QList<Logger *>();

LoggerHelper::LoggerHelper(enum MacrodynError::type e)
{
    ref++;
    if (e != MacrodynError::noError)
    {
        for (int i=0; i < Logger::m_logger.size(); i++)
        {
            Logger::m_logger.at(i)->error(e);
        }
    }
}

LoggerHelper::~LoggerHelper()
{
    ref--;
    if (ref <= 0)
    {
        ref = 0;
        *this << "\n";
    }
}

LoggerHelper& LoggerHelper::operator <<(const QString& str)
{
    for (int i=0; i < Logger::m_logger.size(); i++)
    {
        Logger::m_logger.at(i)->print(str);
    }
    return *this;
}

LoggerHelper& LoggerHelper::operator <<(qreal n)
{
    for (int i=0; i < Logger::m_logger.size(); i++)
    {
        Logger::m_logger.at(i)->print(n);
    }
    return *this;
}

Logger::Logger(QObject *parent) : QObject(parent)
{
    m_logger << this;
    m_precision = 6;
    m_line = "New Log\n";
}

Logger::~Logger()
{
    m_logger.removeOne(this);
}

void Logger::print(const QString &str)
{
    if (str.endsWith("\n"))
    {
        if (m_line.isEmpty())
        {
            if (str != "\n")
            {
                m_oldLine = str;
                m_oldLine.chop(2);
                emit lineChanged();
                qDebug() << m_oldLine;
            }
        }
        else
        {
            m_line += str;
            m_line.chop(2);
            m_oldLine = m_line;
            m_line.clear();
            emit lineChanged();
            qDebug() << m_oldLine;
        }
    }
    else
    {
        m_line += str;
        m_line += " ";
    }
}

void Logger::print(qreal r, int precision)
{
    if (precision <= 0) precision = m_precision;
    m_line += QString::number(r, 'g', precision) += " ";
}

void Logger::setPrecision(int p)
{
    if (p != m_precision)
    {
        m_precision = p;
        print("Precision changed to");
        print(m_precision);
        print("\n");
        emit precisionChanged();
    }
}
