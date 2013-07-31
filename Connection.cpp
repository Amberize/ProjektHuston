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

void Connection::sendFlyingData(QVector<int> data)
{
//    QByteArray msg;
//    msg.append(QString("%1/%2/%3/%4")
//            .arg(QString::number(data[0]))
//            .arg(QString::number(data[1]))
//            .arg(QString::number(data[2]))
//            .arg(QString::number(data[3])));
    write( (char*) data.data(), sizeof( int[4] ) );
}
