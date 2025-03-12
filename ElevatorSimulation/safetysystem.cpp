#include "safetysystem.h"

SafetySystem::SafetySystem(QObject *parent) : QObject(parent), fireAlarmActive(false) {}

void SafetySystem::triggerFireAlarm() {
    fireAlarmActive = true;
    emit updateLog("\n\nFire alarm triggered! Moving all elevators to safe floor (Floor 2).\nAudio: Stay Calm.");
}

void SafetySystem::triggerPowerAlarm() {
    powerAlarmActive = true;
    emit updateLog("\n\nPower Outage detected! Moving all elevators to safe floor (Floor 2).\nAudio: Stay Calm.");
}


bool SafetySystem::isFireAlarmActive() {
    return fireAlarmActive;
}

bool SafetySystem::isPowerAlarmActive() {
    return powerAlarmActive;
}


void SafetySystem::stepFireOrPowerAlarm(QVector<Elevator*> &elevators, QVector<Passenger*> &passengers) {

    // Move elevators step by step
    for (Elevator *elevator : elevators) {
        if (elevator->getCurrentFloor() < safeFloor) {
            elevator->setState(Elevator::MovingUp);
            elevator->moveStep(safeFloor);
        } else if (elevator->getCurrentFloor() > safeFloor) {
            elevator->setState(Elevator::MovingDown);
            elevator->moveStep(safeFloor);
        } else {
            elevator->setState(Elevator::DoorOpen);

            if (!loggedElevators.contains(elevator->getId())) {
                emit updateLog(QString("\n\nElevator %1 reached safe floor (Floor 2).").arg(elevator->getId() + 1));
                loggedElevators.insert(elevator->getId());
            }

            // Evacuate passengers only when their elevator reaches the safe floor
            for (Passenger *passenger : passengers) {
                if (passenger->getElevator() == elevator) {  // Ensure passenger is inside this elevator
                    passenger->updateState(Passenger::Arrived);
                    passenger->setElevator(nullptr);

                    // Log only once per passenger
                    if (!loggedPassengers.contains(passenger->getId())) {
                        emit updateLog(QString("\n\nPassenger %1 evacuated.").arg(passenger->getId() + 1));
                        loggedPassengers.insert(passenger->getId());
                    }
                }
            }
        }
    }
}


void SafetySystem::resetSafetySystem() {
    fireAlarmActive = false;
    powerAlarmActive = false;
    loggedElevators.clear();
    loggedPassengers.clear();
}

