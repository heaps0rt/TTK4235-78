#include "elevator.h"
#include "timer.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    //Initial set up
    elevio_init();
    Elevator elev;
    elevator_init(&elev);       //setter til STATE_INIT

    //Main loop
    while (1) {
        // inputs
        int stopPressed = elevio_stopButton();
        int obstruction = elevio_obstruction();
        int floorSensor = elevio_floorSensor();
        double now = timer_get_current_time();

        // Clear orders on stop button
        if (stopPressed && !elev.stopPressedPrev) {     //so it reacts only to rising edge (stopPressed=1 and stopPressedPrev=0)
            elevator_clear_all_orders(&elev);
        }
        elev.stopPressedPrev = stopPressed;

        // Update orders only if not stopped or initializing
        if (!stopPressed && elev.state != STATE_INIT) {
            elevator_update_orders(&elev);
        }

        // Update lastFloor when arriving at a floor
        if (floorSensor != -1) {
            elev.lastFloor = floorSensor;
        }
        //FSM
        switch (elev.state) {
            case STATE_INIT:
                elev.direction = DIRN_DOWN;     //Move down until a known floor
                if (floorSensor != -1) {        // -1 = between floors
                    elev.direction = DIRN_STOP;
                    elev.currentFloor = floorSensor;
                    elev.displayFloor = floorSensor;
                    elev.state = STATE_IDLE;
                }
                break;

            case STATE_IDLE:                    //i ro, while wating on orders etc
                if (stopPressed) {
                    elev.doorOpen = 1;
                    elev.state = STATE_DOOR_OPEN;
                    elev.doorCloseTime = 0.0;
                } else if (elevator_has_orders(&elev)) {
                    elev.direction = elevator_decide_direction(&elev, elev.currentFloor);
                    if (elev.direction == DIRN_UP) {
                        elev.state = STATE_MOVING_UP;
                    } else if (elev.direction == DIRN_DOWN) {
                        elev.state = STATE_MOVING_DOWN;
                    }
                }
                elev.displayFloor = elev.currentFloor;
                break;

            case STATE_MOVING_UP:
                if (stopPressed) {
                    elev.direction = DIRN_STOP;
                    if (floorSensor != -1) {
                        elev.currentFloor = floorSensor;
                        elev.doorOpen = 1;
                        elev.state = STATE_DOOR_OPEN;
                        elev.doorCloseTime = 0.0;
                    } else {
                        elev.state = STATE_STOPPED_BETWEEN;
                    }
                } else {
                    elev.direction = DIRN_UP;
                    if (floorSensor != -1 && elevator_should_stop_up(&elev, floorSensor)) {
                        elev.direction = DIRN_STOP;
                        elev.currentFloor = floorSensor;
                        elev.doorOpen = 1;
                        elev.state = STATE_DOOR_OPEN;
                        elev.doorCloseTime = now + 3.0;
                        elevator_clear_orders_at_floor(&elev, floorSensor);
                    }
                }
                break;

            case STATE_MOVING_DOWN:
                if (stopPressed) {
                    elev.direction = DIRN_STOP;
                    if (floorSensor != -1) {
                        elev.currentFloor = floorSensor;
                        elev.doorOpen = 1;
                        elev.state = STATE_DOOR_OPEN;
                        elev.doorCloseTime = 0.0;
                    } else {
                        elev.state = STATE_STOPPED_BETWEEN;
                    }
                } else {
                    elev.direction = DIRN_DOWN;
                    if (floorSensor != -1 && elevator_should_stop_down(&elev, floorSensor)) {
                        elev.direction = DIRN_STOP;
                        elev.currentFloor = floorSensor;
                        elev.doorOpen = 1;
                        elev.state = STATE_DOOR_OPEN;
                        elev.doorCloseTime = now + 3.0;
                        elevator_clear_orders_at_floor(&elev, floorSensor);
                    }
                }
                break;

            case STATE_DOOR_OPEN:
                if (stopPressed || obstruction) {
                    elev.doorCloseTime = 0.0;
                } else {
                    if (elev.doorCloseTime == 0.0) {
                        elev.doorCloseTime = now + 3.0;
                    }
                    if (timer_expired(elev.doorCloseTime)) {
                        elev.doorOpen = 0;
                        elev.state = STATE_IDLE;
                        elev.doorCloseTime = 0.0;
                    }
                }
                elev.direction = DIRN_STOP;
                elev.displayFloor = elev.currentFloor;
                break;

            case STATE_STOPPED_BETWEEN:
                elev.direction = DIRN_STOP;
                if (!stopPressed && elevator_has_orders(&elev)) {
                    elev.direction = elevator_decide_direction(&elev, elev.lastFloor);
                    if (elev.direction == DIRN_UP) {
                        elev.state = STATE_MOVING_UP;
                    } else if (elev.direction == DIRN_DOWN) {
                        elev.state = STATE_MOVING_DOWN;
                    }
                }
                break;
        }

        // Set outputs
        elevio_motorDirection(elev.direction);
        elevio_doorOpenLamp(elev.doorOpen);
        elevio_stopLamp(stopPressed);
        if (elev.displayFloor != -1) {      
            elevio_floorIndicator(elev.displayFloor);
        }
        for (int f = 0; f < N_FLOORS; f++) {            //Active orders lamp 
            elevio_buttonLamp(f, BUTTON_HALL_UP, elev.orders[f][BUTTON_HALL_UP]);
            elevio_buttonLamp(f, BUTTON_HALL_DOWN, elev.orders[f][BUTTON_HALL_DOWN]);
            elevio_buttonLamp(f, BUTTON_CAB, elev.orders[f][BUTTON_CAB]);
        }

        usleep(10000); // 10 ms delay
    }

    return 0;
}