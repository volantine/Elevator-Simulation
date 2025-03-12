#include "ControlSystem.h"

ControlSystem::ControlSystem(int numElevators, int numPassengers, QObject *parent)
    : QObject(parent), time(0), safetySystem(new SafetySystem(this)), sensor(new Sensor(this)) {

    connect(safetySystem, &SafetySystem::updateLog, this, &ControlSystem::updateLog);
    connect(sensor, &Sensor::updateLog, this, &ControlSystem::updateLog);

    for (int i = 0; i < numElevators; ++i) {
        Elevator *elevator = new Elevator(i, this);
        elevators.append(elevator);
        connect(elevator, &Elevator::elevatorUpdated, this, &ControlSystem::updateElevator);
    }

    for (int i = 0; i < numPassengers; ++i) {
        Passenger *passenger = new Passenger(i, this);
        passengers.append(passenger);
        connect(passenger, &Passenger::passengerUpdated, this, &ControlSystem::updatePassenger);
    }

    for (Elevator *elevator : elevators) {
        connect(elevator, &Elevator::updateLog, this, &ControlSystem::updateLog);
    }

    connect(&timer, &QTimer::timeout, this, &ControlSystem::stepSimulation);
}

void ControlSystem::start() {
    emit updateLog("Simulation started.");
    timer.start(1000);
}

void ControlSystem::stop() {
    emit updateLog("Simulation stopped.");
    timer.stop();
    resetSimulation();
}

void ControlSystem::pause() {
    paused = true;
    timer.stop();
    emit updateLog("Simulation paused.");
}


void ControlSystem::continueSim() {
    if (paused == true) {
        paused = false;
        emit updateLog("Simulation resumed.");
        timer.start(1000);
    }
}

void ControlSystem::openElevatorDoor(int elevatorId) {
    if (elevatorId >= 0 && elevatorId < elevators.size()) {
        elevators[elevatorId]->openDoor();
    }
}


void ControlSystem::closeElevatorDoor(int elevatorId) {
    if (elevatorId >= 0 && elevatorId < elevators.size()) {
        elevators[elevatorId]->closeDoor();
    }
}

// Can only be pressed in elevators with a passenger
void ControlSystem::Help(int elevatorId) {
    if (elevatorId >= 0 && elevatorId < elevators.size() && elevators[elevatorId]->isOccupied()) {
        elevators[elevatorId]->setState(Elevator::Blocked); // Block the elevator
        emit updateLog(QString("\n\nElevator %1 is blocked due to a safety issue. Connecting to building safety service.\nCall 911 as well? (Yes/No)")
                       .arg(elevators[elevatorId]->getId() + 1));
    }
}


void ControlSystem::triggerFireAlarm() {
    safetySystem->triggerFireAlarm();
}

//  Sensors detect Obstacles
void ControlSystem::ObstacleAlarm(int elevatorId) {
    if (elevatorId >= 0 && elevatorId < elevators.size() && elevators[elevatorId]->isOccupied()) {
        elevators[elevatorId]->setState(Elevator::Blocked); // Block the elevator
        sensor->detectObstacle(elevators[elevatorId]->getId());
    }
}

//  Sensors detect Overloading
void ControlSystem::OverloadingAlarm(int elevatorId) {
    if (elevatorId >= 0 && elevatorId < elevators.size() && elevators[elevatorId]->isOccupied()) {
        elevators[elevatorId]->setState(Elevator::Blocked); // Block the elevator
        sensor->detectOverloading(elevators[elevatorId]->getId());
    }
}


void ControlSystem::triggerPowerAlarm() {
    safetySystem->triggerPowerAlarm();
}

void ControlSystem::resetSimulation() {
    time = 0;
    emit updateTime(time);

    // Reset all elevators to their initial state
    for (Elevator *elevator : elevators) {
        elevator->setCurrentFloor(1);  // Assuming ground floor is 1
        elevator->setState(Elevator::Idle);
        elevator->setOccupied(false);
    }

    // Reset all passengers to their initial state
    for (Passenger *passenger : passengers) {
        passenger->updateState(Passenger::NotRequested);
        passenger->setElevator(nullptr);
    }

    safetySystem->resetSafetySystem();
}

void ControlSystem::setSinglePassenger(int passengerIndex, int startFloor, int destFloor, int timeStep) {
    if (passengerIndex < 0 || passengerIndex >= passengers.size()) return;

    passengers[passengerIndex]->setStartFloor(startFloor);
    passengers[passengerIndex]->setDestFloor(destFloor);
    passengers[passengerIndex]->setRequestTime(timeStep);

    emit updateLog(QString("Passenger %1 set: Start floor %2 -> Destination floor %3 at Time %4")
                   .arg(passengerIndex + 1)
                   .arg(startFloor)
                   .arg(destFloor)
                   .arg(timeStep));
}


void ControlSystem::stepSimulation() {
    time += 1;
    emit updateTime(time);

    if (safetySystem->isFireAlarmActive()) {
        safetySystem->stepFireOrPowerAlarm(elevators, passengers);
        return; // Skip normal operations while alarm is active
    }

    if (safetySystem->isPowerAlarmActive()) {
        safetySystem->stepFireOrPowerAlarm(elevators, passengers);
        return;
    }


    // Step 1: Passengers request an elevator at their specified time
    for (Passenger *passenger : passengers) {
        if (passenger->getRequestTime() == time && passenger->getState() == Passenger::NotRequested) {
            passenger->updateState(Passenger::Requested);
            passenger->ElevatorRequestedOrBoarded();           
        }
    }

    QSet<Elevator*> assignedElevators;

    // Step 2: Assign an elevator to requested passengers
    for (Passenger *passenger : passengers) {
        if (passenger->getState() == Passenger::Requested) {
            Elevator *bestElevator = nullptr;

            // Find the closest elevator that is either idle or already moving toward the passenger
            for (Elevator *elevator : elevators) {
                if (elevator->getState() != Elevator::Blocked && // Skip blocked elevators                        
                    !assignedElevators.contains(elevator) &&
                    !elevator->isOccupied() &&
                    (!bestElevator || abs(elevator->getCurrentFloor() - passenger->getStartFloor()) <
                                      abs(bestElevator->getCurrentFloor() - passenger->getStartFloor()))) {
                    bestElevator = elevator;
                }
            }

            // Move the best elevator toward the passenger
            if (bestElevator) {
                assignedElevators.insert(bestElevator);
                if (passenger->getStartFloor() > bestElevator->getCurrentFloor()) {
                    bestElevator->setState(Elevator::MovingUp);
                    bestElevator->moveStep(passenger->getStartFloor());
                } else if (passenger->getStartFloor() < bestElevator->getCurrentFloor()) {
                    bestElevator->setState(Elevator::MovingDown);
                    bestElevator->moveStep(passenger->getStartFloor());
                } else {
                    // Elevator has arrived at passenger's floor
                    bestElevator->setState(Elevator::DoorOpen);
                    emit updateLog(QString("\n\nElevator %1 arrived at floor %2 for Passenger %3. Bell rings and doors open")
                                                       .arg(bestElevator->getId()+1).arg(bestElevator->getCurrentFloor()).arg(passenger->getId()+1));
                    passenger->updateState(Passenger::Boarding);
                    passenger->setElevator(bestElevator);
                    bestElevator->setOccupied(true);
                }
            }
        }
    }

    // Step 3: Passengers board the elevator
    for (Passenger *passenger : passengers) {
        if (passenger->getState() == Passenger::Boarding && passenger->getElevator() != nullptr) {
            Elevator *assignedElevator = passenger->getElevator();
            if (assignedElevator->getState() != Elevator::Blocked && // Skip blocked elevators
                assignedElevator->getCurrentFloor() == passenger->getStartFloor() && assignedElevator->getState() == Elevator::DoorOpen) {
                passenger->updateState(Passenger::InElevator);
                emit updateLog(QString("\n\nPassenger %1 entered Elevator %2.")
                               .arg(passenger->getId() + 1).arg(assignedElevator->getId() + 1));
                assignedElevator->setState(Elevator::DoorClosed); // Close doors before moving
            }
        }
    }

    // Step 4: Move the elevator to the passenger's destination floor
    for (Passenger *passenger : passengers) {
        if (passenger->getState() == Passenger::InElevator && passenger->getElevator() != nullptr) {
            Elevator *assignedElevator = passenger->getElevator();

            if (assignedElevator->getState() != Elevator::Blocked) { // Skip blocked elevators
                if (assignedElevator->getCurrentFloor() < passenger->getDestFloor()) {
                    assignedElevator->setState(Elevator::MovingUp);
                    assignedElevator->moveStep(passenger->getDestFloor());
                } else if (assignedElevator->getCurrentFloor() > passenger->getDestFloor()) {
                    assignedElevator->setState(Elevator::MovingDown);
                    assignedElevator->moveStep(passenger->getDestFloor());
                } else {
                    assignedElevator->setState(Elevator::DoorOpen);
                    passenger->updateState(Passenger::Arrived);
                    emit updateLog(QString("\n\nElevator %1 arrived at destination floor %2 and Passenger %3 exits.")
                                   .arg(assignedElevator->getId() + 1).arg(assignedElevator->getCurrentFloor()).arg(passenger->getId() + 1));
                    assignedElevator->setOccupied(false); // Mark elevator as available
                    passenger->setElevator(nullptr); // Reset assigned elevator after exit
                    assignedElevators.remove(assignedElevator);
                }
            }
        }
    }
}

