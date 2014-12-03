#ifndef  MACRO_MACHINE_H
#define  MACRO_MACHINE_H

#include <QtCore>


#if defined(QT_NO_FPU) || defined(QT_ARCH_ARM) || defined(QT_ARCH_WINDOWSCE) || defined(QT_ARCH_SYMBIAN)
const unsigned  mantissa_bits = 23;
const qreal  qreal_PRECISION=3.0e-7;   //  >=  2^-mantissa_bits
#else
const unsigned  mantissa_bits = 52;
const qreal  qreal_PRECISION=1.0e-15;   //  >=  2^-mantissa_bits
#endif
unsigned const bits_per_byte = 8;
unsigned const bytes_per_word = 4;
unsigned const bits_per_word = bits_per_byte * bytes_per_word;
#endif
