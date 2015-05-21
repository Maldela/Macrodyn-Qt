#ifndef GET_EXPR_INCLUDED
#define GET_EXPR_INCLUDED

#include <QString>

QString get_expr(QString expr, QString &token, const QRegExp &upto, QChar *ch = NULL);

QString get_expr(QString expr, QString &token, const QChar &upto, QChar *ch = NULL);

QString get_expr(QString expr, QString &token, const QString &upto, QChar *ch = NULL);

#endif // GET_EXPR_INCLUDED
