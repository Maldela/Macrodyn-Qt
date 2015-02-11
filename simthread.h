#ifndef SIMTHREAD_H
#define SIMTHREAD_H

#include <QThread>

#include "Jobs/job.h"

class SimThread : public QThread
{
    Q_OBJECT

public:

    virtual ~SimThread();
    void setJob(Job *job) { m_job = job; }


protected:

    void run();

    Job *m_job;
};

#endif // SIMTHREAD_H
