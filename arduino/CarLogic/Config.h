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
#define CAR_RFID_EXIT_STATE_DEALY 0.008f
#define CAR_RFID_SLOW_DOWN_DELAY 0.2f
#define CAR_RFID_SLOW_DOWN_SPEED 120
#define CAR_RFID_REVERSE_SPEED -100
#define CAR_RFID_RETEST_BRAKE_TIME 0.1f
#define CAR_RFID_RETEST_BRAKE_SPEED -255
#define CAR_RFID_RETEST_ROTATE_SPEED -135, 135
#define CAR_RFID_RETEST_ROTATE_BRAKE_TIME 0.1f
#define CAR_RFID_RETEST_ROTATE_IMMUNITY_TIME 0.2f
#define CAR_RFID_RETEST_SPEED 100
#define CAR_RFID_RETEST_GIVE_UP_TIME 0.45f  // The amount of time to give up re-test RFID after rotate ends  

// In order to prevent detection of the corner of a node, a minimum time required to exit a rotation state is added
#define CAR_ROATATION_EXIT_STATE_IMMUNITY_TIME 0.3f
#define CAR_ROTATE_LEFT_SPEED -120, 120
#define CAR_ROTATE_RIGHT_SPEED 120, -120
#define CAR_ROTATE_STATE_BRAKE_TIME 0.06f

// Path tracing parameters(PID)
#define CAR_PATH_TRACE_PID_P 0.5f
#define CAR_PATH_TRACE_PID_D 0.1f
#define CAR_PATH_TRACE_D_UPDATE_RATE 0.01f
#define CAR_PATH_TRACE_PID_WEIGHT -4.0f, -1.0f, 0.0f, 1.0f, 4.0f

// Path tracing parameters(P-only), used for RFID path trace
#define CAR_PATH_TRACE_ADJUST_STRENGTH 0.25f
#define CAR_PATH_TRACE_ADJUST_WEIGHT -3.5f, -1.0f, 0.0f, 1.0f, 3.5f


//
#define CAR_SPRINT_STATE_IMMUNITY_TIME 0.2f
#define CAR_TURN_STATE_IMMUNITY_TIME 0.35//0.2f
#define CAR_TURN_LEFT_SPEED -255, 255
#define CAR_TURN_RIGHT_SPEED 255, -255

// Brakes to counter the inertia
#define CAR_SPRINT_STATE_BRAKE_TIME 0.02f
#define CAR_SPRINT_STATE_BRAKE_SPEED -255
#define CAR_TURN_BRAKE_TIME 0.02f

#endif