#include "simthread.h"
#include "logger.h"


SimThread::~SimThread()
{
}

void SimThread::run()
{
    if (!m_job)
    {
        error("SimThread:", "No job set!");
        return;
    }
    m_job->simulation();
    m_job = NULL;
}
