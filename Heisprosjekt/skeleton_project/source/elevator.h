#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "driver/elevio.h"  // Får N_FLOORS

typedef enum {
    STATE_INIT,             //Ved oppstart. Beveger ned til nærmeste etasje
    STATE_IDLE,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN,
    STATE_DOOR_OPEN,
    STATE_STOPPED_BETWEEN
} ElevatorState;

typedef struct {
    ElevatorState state;
    int currentFloor;     // -1 if between floors
    int lastFloor;        // Last floor visited. Used for stopping between floors
    int displayFloor;     // Floor light to display
    MotorDirection direction;
    int doorOpen;         // 1 if door is open
    double doorCloseTime; // Time when door should close
    int orders[N_FLOORS][3]; // [floor][BUTTON_HALL_UP, BUTTON_HALL_DOWN, BUTTON_CAB]
    int stopPressedPrev;  // Previous state of stop button
} Elevator;

void elevator_init(Elevator *elev);
void elevator_update_orders(Elevator *elev);
int elevator_has_orders(Elevator *elev);
int elevator_has_orders_above(Elevator *elev, int floor);
int elevator_has_orders_below(Elevator *elev, int floor);
int elevator_should_stop_up(Elevator *elev, int floor);
int elevator_should_stop_down(Elevator *elev, int floor);
MotorDirection elevator_decide_direction(Elevator *elev, int floor);
void elevator_clear_orders_at_floor(Elevator *elev, int floor);
void elevator_clear_all_orders(Elevator *elev);

#endif