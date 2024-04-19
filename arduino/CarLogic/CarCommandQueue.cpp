#include "CarCommandQueue.h"


void CarCommandQueue::EnqueueCommand(CarCommand command)
{
    m_Commands[0] = m_Commands[1];
    m_Commands[1] = command;
    m_CommandCount++;
}

CarCommand CarCommandQueue::NextCommand()
{
    m_CommandCount--;
    return m_Commands[0];
}