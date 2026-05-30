#ifndef GESTUREMANAGER_H
#define GESTUREMANAGER_H

#include "sensor.h"
#include "sensormanager.h"

#include <QMap>
#include <QObject>
#include <QString>
#include <mlite5/MGConfItem>

class GestureManager : public QObject
{
    Q_OBJECT
public:
    explicit GestureManager(SensorManager *manager, QObject *parent = nullptr);
    ~GestureManager();

    void loadSensors();
    void releaseSensors();

public slots:
    void handleEnabledGestureChanged();

private slots:
    void toggleFlashlight(unsigned);
    void launchCamera(unsigned);
    void wakeUp(unsigned);

private:
    SensorManager* _manager;
    QMap<QString, Sensor*> _sensors;
    MGConfItem* _dconfGestures;
};

#endif // GESTUREMANAGER_H
