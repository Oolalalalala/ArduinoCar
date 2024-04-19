#ifndef CAR_COMMAND_QUEUE_H
#define CAR_COMMAND_QUEUE_H


// Values need to be the same in python
enum class CarCommand : unsigned char
{
    None = 0, Forward, TurnLeft, TurnRight, TurnLeft2, TestRFID
};

// A wrapper class for the command queue
class CarCommandQueue
{
public:
    void EnqueueCommand(CarCommand command);
    CarCommand NextCommand();
    int GetCommandCount() { return m_CommandCount; }

private:
    CarCommand m_Commands[2] = {};
    int m_CommandCount = 0;
};


#endif