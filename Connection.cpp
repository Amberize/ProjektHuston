#include "Connection.h"

Connection::Connection(QObject *parent): QTcpSocket(parent)
{
    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
}

void Connection::processReadyRead()
{
}

void Connection::sendPing()
{
    QByteArray msg = "ping";
    write(msg);
}

void Connection::sendFlyingData(int *data)
{
    write( (char*) data, sizeof( int[4] ) );
}
