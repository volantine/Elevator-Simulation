#include "sensor.h"

Sensor::Sensor(QObject *parent) : QObject(parent) {}

void Sensor::detectObstacle(int id) {
    emit updateLog(QString("\n\nWARNING: Elevator %1 door obstruction detected!\nAudio: Please clear the doorway.")
                   .arg(id +1));
}

void Sensor::detectOverloading(int id) {
    emit updateLog(QString("\n\nWARNING: Elevator %1 is overloaded!\nAudio: Please reduce the load before proceeding.")
                   .arg(id +1));
}
