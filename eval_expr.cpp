#include "get_expr.h"
#include "eval_expr.h"
#include "logger.h"

qreal eval_expr(baseModel *model, QString expr)
{
    qreal *value  = NULL;
    int pos = expr.indexOf(QRegExp("+-*/"));
    qreal result = 0;
    qreal val1   = 0;
    qreal val2   = 0;

    if (pos == -1)  // this is an expression
    {
        if ((value = model->setLabels(expr)) != NULL)  // without +-*/
            result = *value;
        else
            result = expr.toDouble();
    }
    else
    {
        QChar op = expr.at(pos);   // operator
        QRegExp exp(op);
        QString token;

        expr = get_expr(expr, token, exp);

        value = model->setLabels(token); // token contains
        if (value != NULL)  // 1st operand
            val1 = *value;
        else
            val1 = token.toDouble();

        value = model->setLabels(expr);
        if (value != NULL)	// 2nd operand
            val2 = *value;
        else
            val2 = expr.toDouble();

        switch (op.toLatin1())
        {
          case '+' : result = val1 + val2; break;
          case '-' : result = val1 - val2; break;
          case '*' : result = val1 * val2; break;
          case '/' : result = val1 / val2; break;
          default  : error("eval_expr: unknown operand: %c", op);
        }
    }
    return result;
}
