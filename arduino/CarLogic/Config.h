#ifndef CONFIG_H
#define CONFIG_H


// The master speed
#define CAR_SPEED 255

// The final ratio that is multiplied on to the calculate speed
#define CAR_LEFT_WHEEL_SPEED_RATIO 0.87f    // <= 1
#define CAR_RIGHT_WHEEL_SPEED_RATIO 1       // <= 1

// The reverse used in TestRFIDState
#define CAR_REVERSE_LEFT_WHEEL_SPEED_RATIO 0.95f
#define CAR_REVERSE_RIGHT_WHEEL_SPEED_RATIO 1

// In order to prevent detection of the corner of a node, a delay is added after car leaves a node
#define CAR_FORWARD_EXIT_STATE_DELAY 0.04f
#define CAR_FORWARD_ENTER_NODE_IMMUNITY_TIME 0.1f

#define CAR_RFID_EXIT_STATE_DEALY 0.1f

// In order to prevent detection of the corner of a node, a minimum time required to exit a rotation state is added
#define CAR_ROATATION_EXIT_STATE_IMMUNITY_TIME 0.3f

// Path tracing coefficients
#define CAR_PATH_TRACE_ADJUST_STRENGTH 0.2f
#define CAR_PATH_TRACE_INFERRED_WEIGHT -4.0f, -1.0f, 0.0f, 1.0f, 4.0f


#endif