#include "socketclient.h"
#include "timedunsigned.h"
#include <QDataStream>
#include <QDebug>

SocketClient::SocketClient(QObject *parent)
    : QObject(parent)
{
    connect(&_socket, &QLocalSocket::readyRead,
            this, &SocketClient::onReadyRead);

//    connect(&_socket,
//            QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error),
//            this,
//            &SocketClient::onError);

    connect(&_socket, &QLocalSocket::connected,
            this, &SocketClient::connected);

    connect(&_socket, &QLocalSocket::disconnected,
            this, &SocketClient::disconnected);
}

void SocketClient::connectToSocket(const QString &path)
{
    _socket.connectToServer(path);
}

void SocketClient::disconnectFromSocket()
{
    _socket.disconnectFromServer();
}

bool SocketClient::isConnected() const
{
    return _socket.state() == QLocalSocket::ConnectedState;
}

void SocketClient::attachSession(int sessionId)
{
    QByteArray packet;
    QDataStream ds(&packet, QIODevice::WriteOnly);
    ds.setByteOrder(QDataStream::LittleEndian);

    ds << quint32(sessionId);

    _socket.write(packet);
    _socket.flush();
}

void SocketClient::onReadyRead()
{
    auto read = [this](void* buffer, int size){
        int bytesRead = 0;
        int retry = 100;

        while (bytesRead < size) {
            int bytes = _socket.read((char *)buffer + bytesRead, size);
            if (bytes == 0) {
                if (!retry)
                    return false;
                struct timespec ts;
                ts.tv_sec = 0;
                ts.tv_nsec = 100000000;
                nanosleep(&ts, NULL);
                --retry;
                continue;
            }
            if (bytes < 1)
                return false;
            bytesRead += bytes;
        }
        return bytesRead > 0;
    };

    QVector<TimedUnsigned> values;

    unsigned int count;
    if (!read((void*)&count, sizeof(unsigned int))) {
        _socket.readAll();
        return;
    }
    if (count > 1000) {
        qWarning() << "Too many samples waiting in socket. Flushing it to empty";
        _socket.readAll();
        return;
    }

    values.resize(values.size() + count);
    if (!read((void*)values.data(), sizeof(TimedUnsigned) * count)) {
        qWarning() << "Error occured while reading data from socket: " << _socket.errorString();
        _socket.readAll();
        return;
    }

    foreach (const TimedUnsigned &data, values)
        emit dataReceived(data.value_);
}

void SocketClient::onError(QLocalSocket::LocalSocketError)
{
    emit errorOccurred(_socket.errorString());
}
