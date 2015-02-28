#ifndef RANLIB_H
#define RANLIB_H

#include <QtCore>

inline qreal ranf()
{
    return (qreal)rand() / RAND_MAX;
}

#endif // RANLIB_H
