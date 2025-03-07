#include "elevator.h"
#include <stdbool.h>

void elevator_init(Elevator *elev) {
    elev->state = STATE_INIT;
    elev->currentFloor = -1;
    elev->lastFloor = -1;
    elev->displayFloor = -1;
    elev->direction = DIRN_STOP;
    elev->doorOpen = 0;
    elev->doorCloseTime = 0.0;
    elev->stopPressedPrev = 0;
    for (int f = 0; f < N_FLOORS; f++) {
        elev->orders[f][BUTTON_HALL_UP] = 0;
        elev->orders[f][BUTTON_HALL_DOWN] = 0;
        elev->orders[f][BUTTON_CAB] = 0;
    }
}

void elevator_update_orders(Elevator *elev) {
    for (int f = 0; f < N_FLOORS; f++) {
        if (elevio_callButton(f, BUTTON_HALL_UP)) {
            elev->orders[f][BUTTON_HALL_UP] = 1;
        }
        if (elevio_callButton(f, BUTTON_HALL_DOWN)) {
            elev->orders[f][BUTTON_HALL_DOWN] = 1;
        }
        if (elevio_callButton(f, BUTTON_CAB)) {
            elev->orders[f][BUTTON_CAB] = 1;
        }
    }
}

void elevator_clear_orders_at_floor(Elevator *elev, int floor) {
    if (floor >= 0 && floor < N_FLOORS) {
        elev->orders[floor][BUTTON_HALL_UP] = 0;
        elev->orders[floor][BUTTON_HALL_DOWN] = 0;
        elev->orders[floor][BUTTON_CAB] = 0;
    }
}

void elevator_clear_all_orders(Elevator *elev) {
    for (int f = 0; f < N_FLOORS; f++) {
        elevator_clear_orders_at_floor(elev, f);
    }
}

int elevator_has_orders(Elevator *elev) {
    for (int f = 0; f < N_FLOORS; f++) {
        if (elev->orders[f][BUTTON_HALL_UP] || elev->orders[f][BUTTON_HALL_DOWN] || elev->orders[f][BUTTON_CAB]) {
            return 1;
        }
    }
    return 0;
}

int elevator_has_orders_above(Elevator *elev, int floor) {
    for (int f = floor + 1; f < N_FLOORS; f++) {
        if (elev->orders[f][BUTTON_HALL_UP] || elev->orders[f][BUTTON_HALL_DOWN] || elev->orders[f][BUTTON_CAB]) {
            return 1;
        }
    }
    return 0;
}

int elevator_has_orders_below(Elevator *elev, int floor) {
    for (int f = 0; f < floor; f++) {
        if (elev->orders[f][BUTTON_HALL_UP] || elev->orders[f][BUTTON_HALL_DOWN] || elev->orders[f][BUTTON_CAB]) {
            return 1;
        }
    }
    return 0;
}

int elevator_should_stop_up(Elevator *elev, int floor) {
    if (floor < 0 || floor >= N_FLOORS) return 0;
    return (elev->orders[floor][BUTTON_CAB] ||
            elev->orders[floor][BUTTON_HALL_UP] ||
            (!elevator_has_orders_above(elev, floor) && elev->orders[floor][BUTTON_HALL_DOWN]));
}

int elevator_should_stop_down(Elevator *elev, int floor) {
    if (floor < 0 || floor >= N_FLOORS) return 0;
    return (elev->orders[floor][BUTTON_CAB] ||
            elev->orders[floor][BUTTON_HALL_DOWN] ||
            (!elevator_has_orders_below(elev, floor) && elev->orders[floor][BUTTON_HALL_UP]));
}

MotorDirection elevator_decide_direction(Elevator *elev, int floor) {
    if (elevator_has_orders_above(elev, floor)) {
        return DIRN_UP;
    } else if (elevator_has_orders_below(elev, floor)) {
        return DIRN_DOWN;
    }
    return DIRN_STOP;
}