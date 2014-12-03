#include "error.h"
#include "log.h"

void fatalError(const QString &where, const QString &what)
{
    log() << QString("Fatal error:") << where << " " << what << endl;
}

// eof

void error(const QString &arg1, const QString &arg2)
{
    log() << QString("Error:") << arg1 << " " << arg2 << endl;
}
