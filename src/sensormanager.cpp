#include "sensormanager.h"

#include <QCoreApplication>
#include <QDebug>

SensorManager::SensorManager(QObject *parent) : QObject(parent),
    _iface("com.nokia.SensorService", "/SensorManager", "local.SensorManager", QDBusConnection::systemBus())
{
    QDBusConnection::systemBus().connect(
        "com.nokia.SensorService",
        "/SensorManager",
        "local.SensorManager",
        "errorSignal",
        this,
        SIGNAL(errorSignal(int))
    );
}

QStringList SensorManager::availablePlugins()
{
    QDBusReply<QStringList> reply = _iface.call("availablePlugins");
    return reply.isValid() ? reply.value() : QStringList();
}

QStringList SensorManager::availableSensorPlugins()
{
    QDBusReply<QStringList> reply = _iface.call("availableSensorPlugins");
    return reply.isValid() ? reply.value() : QStringList();
}

bool SensorManager::loadPlugin(const QString &name)
{
    QDBusReply<bool> reply = _iface.call("loadPlugin", QVariant(name));
    return reply.isValid() && reply.value();
}

bool SensorManager::pluginAvailable(const QString &name)
{
    QDBusReply<bool> reply = _iface.call("pluginAvailable", QVariant(name));
    return reply.isValid() && reply.value();
}

int SensorManager::requestSensor(const QString &id)

{
    QDBusReply<int> reply = _iface.call(
        "requestSensor",
        id,
        (qlonglong)QCoreApplication::applicationPid()
        );

    if (!reply.isValid()) {
        qWarning() << "requestSensor failed:" << reply.error().message();
        return -1;
    }

    return reply.value();
}

bool SensorManager::releaseSensor(const QString &id, int sessionId)

{
    QDBusReply<bool> reply = _iface.call(
        "releaseSensor",
        id,
        sessionId,
        (qlonglong)QCoreApplication::applicationPid()
        );

    return reply.isValid() && reply.value();
}
