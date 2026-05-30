#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>

class SensorManager : public QObject
{
    Q_OBJECT

public:
    explicit SensorManager(QObject *parent = nullptr);

    QStringList availablePlugins();
    QStringList availableSensorPlugins();

    bool loadPlugin(const QString &name);
    bool pluginAvailable(const QString &name);

    int requestSensor(const QString &id);
    bool releaseSensor(const QString &id, int sessionId);

    int errorCode() const;
    QString errorString() const;

signals:
    void errorSignal(int error);

private:
    QDBusInterface _iface;
};

#endif // SENSORMANAGER_H
