#include "sensor.h"
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QVariantMap>
#include <QDebug>

Sensor::Sensor(int sessionId, QString sensor, QObject *parent)
    : QObject(parent)
    , _sessionId(sessionId), _sensor(sensor)
{
    connect(&_client, &SocketClient::dataReceived, this, &Sensor::triggered);

    auto getInterface = [](QString sensor){
        return QString(sensor == "chopchopsensor" ? "ChopChopSensor" :
               sensor == "cameragesturesensor" ? "CameraGestureSensor" :
               sensor == "motoglancesensor" ? "MotoGlanceSensor" :
               sensor == "liftgesturesensor" ? "LiftGestureSensor" : "");
    };

    _iface = new QDBusInterface("com.nokia.SensorService",
          "/SensorManager/" + sensor,
          "local." + getInterface(sensor),
          QDBusConnection::systemBus());

    _client.connectToSocket();
    _client.attachSession(_sessionId);
}

Sensor::~Sensor()
{
    delete _iface;
}

bool Sensor::isValid()
{
    QVariant reply = _iface->property("isValid");
    return reply.isValid() && reply.toBool();
}

uint Sensor::bufferInterval() const
{
    return _iface->property("bufferInterval").toUInt();
}

uint Sensor::bufferSize() const
{
    return _iface->property("bufferSize").toUInt();
}

QString Sensor::description() const
{
    return _iface->property("description").toString();
}

QString Sensor::errorString() const
{
    return _iface->property("errorString").toString();
}

QString Sensor::id() const
{
    return _iface->property("id").toString();
}

uint Sensor::interval() const
{
    return _iface->property("interval").toUInt();
}

bool Sensor::hwBuffering() const
{
    return _iface->property("hwBuffering").toBool();
}

bool Sensor::standbyOverride() const
{
    return _iface->property("standbyOverride").toBool();
}

QString Sensor::type() const
{
    return _iface->property("type").toString();
}

int Sensor::errorCode() const
{
    return _iface->property("errorCodeInt").toInt();
}

void Sensor::start()
{
    _iface->call("start", QVariant(_sessionId));
}

void Sensor::stop()
{
    _iface->call("stop", QVariant(_sessionId));
}

int Sensor::getSessionId() const
{
    return _sessionId;
}

QString Sensor::getSensorType() const
{
    return _sensor;
}

QVector<QVariant> Sensor::getAvailableIntervals()
{
    QDBusReply<QVariant> reply = _iface->call("getAvailableIntervals");

    QVector<QVariant> out;

    if (!reply.isValid())
        return out;

    QDBusArgument arg = reply.value().value<QDBusArgument>();

    arg.beginArray();
    while (!arg.atEnd()) {
        QVariant v;
        arg >> v;
        out.append(v);
    }
    arg.endArray();


    return out;
}
void Sensor::setInterval(int interval_ms)
{
    _iface->call("setInterval",
                QVariant(_sessionId),
                QVariant(interval_ms));
}

void Sensor::setDataRate(double dataRate_Hz)
{
    _iface->call("setDataRate",
                QVariant(_sessionId),
                QVariant(dataRate_Hz));
}

void Sensor::setBufferSize(uint value)
{
    _iface->call("setBufferSize",
                QVariant(_sessionId),
                QVariant(value));
}

void Sensor::setBufferInterval(uint interval_ms)
{
    _iface->call("setBufferInterval",
                QVariant(_sessionId),
                QVariant(interval_ms));
}

bool Sensor::setDefaultInterval()
{
    QDBusReply<bool> reply = _iface->call("setDefaultInterval",
                                         QVariant(_sessionId));
    return reply.isValid() && reply.value();
}

bool Sensor::setStandbyOverride(bool value)
{
    QDBusReply<bool> reply = _iface->call("setStandbyOverride",
                                         QVariant(_sessionId),
                                         QVariant(value));
    return reply.isValid() && reply.value();
}
