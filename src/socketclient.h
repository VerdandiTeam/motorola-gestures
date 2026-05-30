#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>

#include <QtNetwork/qlocalsocket.h>

class SocketClient : public QObject
{
    Q_OBJECT

public:
    explicit SocketClient(QObject *parent = nullptr);

    void connectToSocket(const QString &path = "/run/sensord.sock");
    void disconnectFromSocket();

    bool isConnected() const;

    void attachSession(int sessionId);

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString &error);

    void dataReceived(unsigned value);

private slots:
    void onReadyRead();
    void onError(QLocalSocket::LocalSocketError err);

private:
    QLocalSocket _socket;
    QByteArray _buffer;
};

#endif // SOCKETCLIENT_H
