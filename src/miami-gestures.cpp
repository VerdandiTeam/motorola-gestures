#include "gesturemanager.h"
#include "sensor.h"
#include "sensormanager.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTime>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SensorManager sensors;
    GestureManager gestures(&sensors);

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&gestures](){
        qDebug() << "Quitting";

        gestures.releaseSensors();
    });

    gestures.loadSensors();

    return app.exec();
}
