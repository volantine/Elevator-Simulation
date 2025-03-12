#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>

class Sensor : public QObject {
    Q_OBJECT

public:
    explicit Sensor(QObject *parent = nullptr);
    void detectObstacle(int id);  // Function to detect obstacle
    void detectOverloading(int id);  // Function to detect overloading

signals:
    void updateLog(const QString &message);  // Signal to log warnings
};

#endif // SENSOR_H
