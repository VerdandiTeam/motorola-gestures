#include "gesturemanager.h"

#include <QCoreApplication>
#include <QTime>

GestureManager::GestureManager(SensorManager *manager, QObject *parent) : QObject(parent), _manager(manager)
{
    _dconfGestures = new MGConfItem("/apps/motogestures/enabled-gestures");

    connect(_dconfGestures, &MGConfItem::valueChanged, this, &GestureManager::handleEnabledGestureChanged);
}

GestureManager::~GestureManager()
{
    for (Sensor* sensor : _sensors) {
        delete sensor;
    }
}

void GestureManager::releaseSensors()
{
    for (Sensor* sensor : _sensors) {
        sensor->stop();
        _manager->releaseSensor(sensor->getSensorType(), sensor->getSessionId());
    }
}

void GestureManager::handleEnabledGestureChanged()
{
    QStringList enabledGestures = _dconfGestures->value(QStringList({"chopchopsensor", "cameragesturesensor", "liftgesturesensor"})).toStringList();

    qDebug() << "Currently enabled gestures:" << enabledGestures;

    for (Sensor* sensor : _sensors) {
        if (enabledGestures.contains(sensor->getSensorType())) {
            qDebug() << "Enabling gesture " << sensor->getSensorType();
            sensor->start();
            sensor->setStandbyOverride(true);
        } else {
            qDebug() << "Disabling gesture " << sensor->getSensorType();
            sensor->stop();
            sensor->setStandbyOverride(false);
        }
    }
}

void GestureManager::toggleFlashlight(unsigned)
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage call = QDBusMessage::createMethodCall("com.jolla.settings.system.flashlight",
                                                       "/com/jolla/settings/system/flashlight",
                                                       "com.jolla.settings.system.flashlight",
                                                       "toggleFlashlight");

    QDBusError error = bus.call(call);
    if (error.isValid())
        qWarning() << "Flashlight toggle failed:" << error.message();
}

void GestureManager::launchCamera(unsigned)
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage call = QDBusMessage::createMethodCall("com.jolla.camera",
                                                       "/",
                                                       "com.jolla.camera.ui",
                                                       "showViewfinder");

    QVariantList args;
    args << QString("");
    call.setArguments(args);

    bus.call(call, QDBus::NoBlock, 1);
}

void GestureManager::wakeUp(unsigned)
{
    QDBusConnection bus = QDBusConnection::systemBus();
    QDBusMessage call = QDBusMessage::createMethodCall("com.nokia.mce",
                                                       "/com/nokia/mce/request",
                                                       "com.nokia.mce.request",
                                                       "req_display_state_on");

    QDBusError error = bus.call(call);
    if (error.isValid())
        qWarning() << "Turning screen on failed:" << error.message();
}

void GestureManager::loadSensors()
{
    QStringList sensors = {"chopchopsensor", "cameragesturesensor", "liftgesturesensor"};

    for (QString sensor : sensors) {
        if (!_manager->pluginAvailable(sensor)) {
            qCritical() << "Plugin " << sensor << " not available";
            continue;
        }

        if (!_manager->loadPlugin(sensor)) {
            qCritical() << "Failed to load plugin " << sensor;
            continue;
        }

        auto session = _manager->requestSensor(sensor);
        qDebug() << "Plugin session id: " << session;

        if (session == -1) {
            qCritical() << "Failed to obtain plugin " << sensor;
            continue;
        }

        _sensors[sensor] = new Sensor(session, sensor);

        if (sensor == "chopchopsensor") {
            connect(_sensors[sensor], &Sensor::triggered, this, &GestureManager::toggleFlashlight);
        } else if (sensor == "cameragesturesensor") {
            connect(_sensors[sensor], &Sensor::triggered, this, &GestureManager::launchCamera);
        } else {
            connect(_sensors[sensor], &Sensor::triggered, this, &GestureManager::wakeUp);
        }
    }

    handleEnabledGestureChanged();
}
