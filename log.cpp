#include "log.h"

Log * Log::m_log = 0;

Log::Log(QQuickItem *parent) : QQuickItem(parent)
{
    m_log = this;
    m_precision = 6;
}

void Log::print(const QString &str)
{
    if (str == "\n")
    {
        emit newLine(m_line);
        m_line = "";
    }
    else
    {
        m_line += str;
        m_line += " ";
    }
}

void Log::print(qreal r, int precision)
{
    if (precision == 0) precision = m_precision;
    m_line += QString::number(r, 'g', precision) += " ";
}

void Log::setPrecision(int p)
{
    if (p != m_precision)
    {
        m_precision = p;
        emit precisionChanged();
    }
}
