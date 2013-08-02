#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>

class Connection: public QTcpSocket
{
    Q_OBJECT

public:
    Connection(QObject *parent = 0);

    void sendFlyingData(int* data);

public slots:
    void sendPing();

private slots:
    void processReadyRead();
};

#endif // CONNECTION_H
