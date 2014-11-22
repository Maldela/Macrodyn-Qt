#ifndef RANLIB_H
#define RANLIB_H

#include <QtCore>

inline qreal ranf()
{
    return (qreal)random() / RAND_MAX;
}

#endif // RANLIB_H
