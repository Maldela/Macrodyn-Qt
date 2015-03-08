#ifndef MACROSTRING
#define MACROSTRING

struct MacroString
{
    MacroString(const QString& s, const QPointF& p, const QColor& c, bool t)
    {
        string = s;
        point = p;
        color = c;
        transform = t;
    }

    QString string;
    QPointF point;
    QColor color;
    bool transform;
};

#endif // MACROSTRING

