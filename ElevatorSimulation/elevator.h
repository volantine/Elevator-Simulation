#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>

class Elevator : public QObject {
    Q_OBJECT

public:
    enum State { Idle, MovingUp, MovingDown, DoorOpen, DoorClosed, Blocked};

    explicit Elevator(int id, QObject *parent = nullptr);
    void moveStep(int x);
    void setState(State s);
    void setCurrentFloor(int floor);
    int getCurrentFloor() const;
    State getState();
    int getId();
    bool isOccupied();
    void setOccupied(bool status);
    void openDoor();
    void closeDoor();

signals:
    void elevatorUpdated(int id, int floor, Elevator::State state);
    void updateLog(const QString &message);

private:
    int id;
    int currentFloor;
    State state;
    bool occupied = false;
};

#endif // ELEVATOR_H
