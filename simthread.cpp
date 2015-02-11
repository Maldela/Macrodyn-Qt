#include "simthread.h"
#include "logger.h"


SimThread::~SimThread()
{
    if (m_job) delete m_job;
}

void SimThread::run()
{
    if (!m_job)
    {
        error("SimThread:", "No job set!");
        return;
    }
    m_job->simulation();
    delete m_job;
    m_job = NULL;
}
