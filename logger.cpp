#include "logger.h"

QList<Logger *> Logger::m_logger = QList<Logger *>();

LoggerHelper& LoggerHelper::operator <<(const QString& str)
{
    for (int i=0; i<Logger::m_logger.size(); i++)
    {
        Logger::m_logger[i]->print(str);
    }
    return *this;
}

LoggerHelper& LoggerHelper::operator <<(qreal n)
{
    for (int i=0; i<Logger::m_logger.size(); i++)
    {
        Logger::m_logger[i]->print(n);
    }
    return *this;
}

Logger::Logger(QObject *parent) : QObject(parent)
{
    m_logger << this;
    m_precision = 6;
}

Logger::~Logger()
{
    m_logger.removeOne(this);
}

void Logger::print(const QString &str)
{
    if (str == "\n")
    {
        emit lineChanged();
        m_oldLine = m_line;
        m_line = "";
    }
    else
    {
        m_line += str;
        m_line += " ";
    }
}

void Logger::print(qreal r, int precision)
{
    if (precision == 0) precision = m_precision;
    m_line += QString::number(r, 'g', precision) += " ";
}

void Logger::setPrecision(int p)
{
    if (p != m_precision)
    {
        m_precision = p;
        emit precisionChanged();
    }
}
