#include "get_expr.h"

#include <QRegExp>

QString get_expr(QString expr, QString &token, const QRegExp &upto, QChar *ch)
{
    int pos = expr.indexOf(upto);

    if (pos == -1)  // end of string reached
        return QString();
    else if (ch != NULL)
        *ch = expr.at(pos);

    token = expr.left(pos);

    return expr.remove(0, pos+1);			// skip the 'upto'
}

QString get_expr(QString expr, QString &token, const QChar &upto, QChar *ch)
{
    return get_expr(expr, token, QRegExp(upto), ch);
}

QString get_expr(QString expr, QString &token, const QString &upto, QChar *ch)
{
    return get_expr(expr, token, QRegExp(upto), ch);
}
