#include "passenger.h"

Passenger::Passenger(int id, QObject *parent)
    : QObject(parent), id(id), startFloor(1), destFloor(1), requestTime(0), state(NotRequested), assignedElevator(nullptr) {}

void Passenger::setStartFloor(int floor) {
    startFloor = floor;
}

void Passenger::setDestFloor(int floor) {
    destFloor = floor;
}

void Passenger::setElevator(Elevator *elevator) {
    assignedElevator = elevator;
}

void Passenger::setRequestTime(int time) {
    requestTime = time;
}

int Passenger::getStartFloor() {
    return startFloor;
}

int Passenger::getDestFloor() {
    return destFloor;
}

int Passenger::getId() {
    return id;
}

Passenger::State Passenger::getState() {
    return state;
}

Elevator* Passenger::getElevator() const {
    return assignedElevator;
}

int Passenger::getRequestTime() const {
    return requestTime;
}

void Passenger::ElevatorRequestedOrBoarded() {
    emit passengerUpdated(id, startFloor, state);
}

void Passenger::updateState(State s) {
    state = s;
}
