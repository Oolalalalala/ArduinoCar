#ifndef CONFIG_H
#define CONFIG_H


// The master speed
#define CAR_SPEED 255

// The final ratio that is multiplied on to the calculate speed
#define CAR_LEFT_WHEEL_SPEED_RATIO 1        // <= 1
#define CAR_RIGHT_WHEEL_SPEED_RATIO 1       // <= 1

// The reverse speed used in TestRFIDState
#define CAR_REVERSE_LEFT_WHEEL_SPEED_RATIO 1
#define CAR_REVERSE_RIGHT_WHEEL_SPEED_RATIO 1

// In order to prevent detection of the corner of a node, a delay is added after car leaves a node
#define CAR_FORWARD_EXIT_STATE_DELAY 0.02f
#define CAR_FORWARD_ENTER_NODE_IMMUNITY_TIME 0.1f

// RFID
#define CAR_RFID_EXIT_STATE_DEALY 0.05f
#define CAR_RFID_FULL_SPEED_DURATION 0.5f
#define CAR_RFID_SLOW_SPEED 150

// In order to prevent detection of the corner of a node, a minimum time required to exit a rotation state is added
#define CAR_ROATATION_EXIT_STATE_IMMUNITY_TIME 0.15f

// Path tracing parameters
#define CAR_PATH_TRACE_ADJUST_P 0.5f
#define CAR_PATH_TRACE_ADJUST_D 0.1f
#define CAR_BACK_TRACE_ADJUST_D 0.85f
#define CAR_PATH_TRACE_INFERRED_WEIGHT -4.0f, -1.0f, 0.0f, 1.0f, 4.0f

//
#define CAR_SPRINT_STATE_IMMUNITY_TIME 0.2f
#define CAR_TURN_STATE_IMMUNITY_TIME 0.29//0.2f
#define CAR_TURN_LEFT_SPEED -255, 255
#define CAR_TURN_RIGHT_SPEED 255, -255

// Brakes to counter the inertia
#define CAR_SPRINT_STATE_BRAKE_TIME 0.02f
#define CAR_SPRINT_STATE_BRAKE_SPEED 255
#define CAR_TURN_BRAKE_TIME 0.00f

// PID Related
#define D_UPDATE_RATE 0.01f

#endif