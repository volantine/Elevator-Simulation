#include "elevator.h"

Elevator::Elevator(int id, QObject *parent)
    : QObject(parent), id(id), currentFloor(1), state(Idle) {}

void Elevator::moveStep(int floor) {
    if (state == MovingUp) {
        currentFloor++;
    } else if (state == MovingDown) {
        currentFloor--;
    }

    if (currentFloor != floor) {
        emit elevatorUpdated(id, currentFloor, state);
    }
}

int Elevator::getId() {
    return id;
}

Elevator::State Elevator::getState() {
    return state;
}

int Elevator::getCurrentFloor() const {
    return currentFloor;
}

void Elevator::setState(State s) {
    state = s;
}

void Elevator::setCurrentFloor(int floor) {
    currentFloor = floor;
}

bool Elevator::isOccupied() {
    return occupied;
}

void Elevator::setOccupied(bool status) {
    occupied = status;
}

void Elevator::openDoor() {
    if (state == MovingUp || state == MovingDown) {
        emit updateLog(QString("Elevator %1: Cannot open doors while moving!").arg(id + 1));
        return;
    }

    if (state == Idle || state == DoorClosed) {
        setState(DoorOpen);
        emit updateLog(QString("Elevator %1: Doors opened.").arg(id + 1));
    }
}

void Elevator::closeDoor() {
    if (state == DoorOpen) {
        setState(DoorClosed);
        emit updateLog(QString("Elevator %1: Doors closed.").arg(id + 1));
    }
}






