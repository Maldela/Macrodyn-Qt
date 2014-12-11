#include "error.h"
#include "logger.h"

void fatalError(const QString &where, const QString &what)
{
    log() << QString("Fatal error:") << where << " " << what;
}

void fatalError(const char *where, const char *what)
{
    log() << QString("Fatal error:") << QString(where) << " " << QString(what);
}

void error(const QString &arg1, const QString &arg2)
{
    log() << QString("Error:") << arg1 << " " << arg2;
}
