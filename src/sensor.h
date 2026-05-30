#ifndef SENSOR_H
#define SENSOR_H

#include "socketclient.h"

#include <QObject>
#include <QDBusInterface>
#include <QVariant>
#include <QString>
#include <QVector>

class Sensor : public QObject
{
    Q_OBJECT

public:
    explicit Sensor(int sessionId, QString sensor, QObject *parent = nullptr);
    ~Sensor();

    bool isValid();

    uint bufferInterval() const;
    uint bufferSize() const;
    QString description() const;
    QString errorString() const;
    QString id() const;
    uint interval() const;
    bool hwBuffering() const;
    bool standbyOverride() const;
    QString type() const;

    int errorCode() const;

    void setInterval(int interval_ms);
    void setDataRate(double dataRate_Hz);
    void setBufferSize(uint value);
    void setBufferInterval(uint interval_ms);

    bool setDefaultInterval();
    bool setStandbyOverride(bool value);
    void setDownsampling(bool value);

    void start();
    void stop();

    int getSessionId() const;
    QString getSensorType() const;

    QVector<QVariant> getAvailableBufferIntervals() const;
    QVector<QVariant> getAvailableBufferSizes() const;
    QVector<QVariant> getAvailableDataRanges() const;
    QVector<QVariant> getAvailableIntervals();

    QVariant getCurrentDataRange() const;

    void requestDataRange(const QVariant &range);
    void removeDataRangeRequest();
    bool setDataRangeIndex(int rangeIndex);

signals:
    void triggered(unsigned);

private:
    QDBusInterface* _iface;
    SocketClient _client;
    int _sessionId;
    QString _sensor;
};
#endif // SENSOR_H
