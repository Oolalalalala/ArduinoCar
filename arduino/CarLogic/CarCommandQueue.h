#ifndef CAR_COMMAND_QUEUE_H
#define CAR_COMMAND_QUEUE_H


// Values need to be the same in python
enum class CarCommand : unsigned char
{
    None = 0, Forward, Backward, TurnLeft, TurnRight, RotateLeft, RotateRight,
    TurnLeft1 = TurnLeft, TurnRight1 = TurnRight // Used in non-extreme mode
};

// A wrapper class for the command queue
class CarCommandQueue
{
public:
    void EnqueueCommand(CarCommand command);
    CarCommand NextCommand();

private:
    CarCommand m_Commands[2] = {};
};


#endif