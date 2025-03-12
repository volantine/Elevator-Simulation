#ifndef PASSENGER_H
#define PASSENGER_H

#include <QObject>

class Elevator;

class Passenger : public QObject {
    Q_OBJECT

public:
    enum State {NotRequested, Requested, Boarding, InElevator, Arrived};

    explicit Passenger(int id, QObject *parent = nullptr);
    void setStartFloor(int floor);
    void setDestFloor(int floor);
    void setRequestTime(int time);
    int getStartFloor();
    int getDestFloor();
    int getRequestTime() const;
    void requestElevator();
    void updateState(State s);
    State getState();
    void ElevatorRequestedOrBoarded();
    int getId();
    void setElevator(Elevator *elevator);
    Elevator* getElevator() const;

signals:
    void passengerUpdated(int id, int floor, Passenger::State state);

private:
    int id;
    int startFloor;
    int destFloor;
    int requestTime;
    State state;
    Elevator *assignedElevator = nullptr;
};

#endif // PASSENGER_H
