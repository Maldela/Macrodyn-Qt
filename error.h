// File:	error.h
// Contents:	declaration of function fatalError
// Author:	Uli Middelberg
// Creation:	Mon Oct 28 20:40:04 MEZ 1996
// Modified:	Tue Oct 29 15:16:47 MEZ 1996
// Changes:	

#ifndef ERROR_H
#define ERROR_H

#include <QString>

void fatalError(const QString&, const QString&);
void error(const QString&, const QString &arg2 = "");

#endif
//eof
