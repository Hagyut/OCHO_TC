#ifndef GTTCPSOCKET_H
#define GTTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QString>

#include "gttcpsocketinterface.h"

class GtTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit GtTcpSocket(QObject *parent = 0);
    void setTcpSocket(QString url, int port, GtTcpSocketInterface *intf);
    void doConnect();
    void write(char* msg);
    void release() {
        socket->close();
        delete socket;
        this->~GtTcpSocket();
    }

private:
    ~GtTcpSocket() {

    }

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
    GtTcpSocketInterface *intf;
    QString conn_url;
    int conn_port;
};

#endif // GTTCPSOCKET_H
