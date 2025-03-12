#ifndef CONTROLSYSTEM_H
#define CONTROLSYSTEM_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include "elevator.h"
#include "passenger.h"
#include "sensor.h"
#include "safetysystem.h"

class ControlSystem : public QObject {
    Q_OBJECT

public:
    explicit ControlSystem(int numElevators, int numPassengers, QObject *parent = nullptr);
    void start();
    void stop();
    void pause();
    void continueSim();
    void resetSimulation();
    void setSinglePassenger(int passengerIndex, int startFloor, int destFloor, int timeStep);
    void triggerFireAlarm();
    void triggerPowerAlarm();
    void Help(int elevatorId);
    void openElevatorDoor(int elevatorId);
    void closeElevatorDoor(int elevatorId);
    void ObstacleAlarm(int elevatorId);
    void OverloadingAlarm(int elevatorId);

signals:
    void updateLog(const QString &message);
    void updateTime(int time);
    void updateElevator(int id, int floor, Elevator::State state);
    void updatePassenger(int id, int floor, Passenger::State state);

private slots:
    void stepSimulation();

private:
    int time;
    bool paused = false;
    QVector<Elevator*> elevators;
    QVector<Passenger*> passengers;
    SafetySystem *safetySystem;
    Sensor *sensor;
    QTimer timer;
};

#endif // CONTROLSYSTEM_H
