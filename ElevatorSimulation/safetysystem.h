#ifndef SAFETYSYSTEM_H
#define SAFETYSYSTEM_H

#include <QObject>
#include <QTimer>
#include <QSet>
#include "elevator.h"
#include "passenger.h"

class SafetySystem : public QObject {
    Q_OBJECT

public:
    explicit SafetySystem(QObject *parent = nullptr);
    void triggerFireAlarm();
    void triggerPowerAlarm();
    void stepFireOrPowerAlarm(QVector<Elevator*> &elevators, QVector<Passenger*> &passengers);
    bool isFireAlarmActive();
    bool isPowerAlarmActive();
    void resetSafetySystem();
signals:
    void updateLog(const QString &message);

private:
    bool fireAlarmActive = false;
    bool powerAlarmActive = false;
    const int safeFloor = 2;
    QSet<int> loggedElevators;
    QSet<int> loggedPassengers;
};

#endif // SAFETYSYSTEM_H
